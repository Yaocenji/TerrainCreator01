#include "renderer.h"

#include "Global/globalgl.h"
#include "Global/globalrender.h"
#include "Global/globalui.h"

#define PreRenderTerrainGround(framebuffer)                     \
    {                                                           \
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);         \
        glClearColor(globalrender::backgroundColor.redF(),      \
                     globalrender::backgroundColor.greenF(),    \
                     globalrender::backgroundColor.blueF(), 1); \
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);     \
        glEnable(GL_DEPTH_TEST);                                \
        glDisable(GL_BLEND);                                    \
    }

#define PreSwapFrameBufferToScreen(colorbuffer, depthbuffer)           \
    {                                                                  \
        glBindFramebuffer(GL_FRAMEBUFFER, defaultFramebufferObject()); \
        glClear(GL_COLOR_BUFFER_BIT);                                  \
        glDisable(GL_DEPTH_TEST);                                      \
        glActiveTexture(GL_TEXTURE0);                                  \
        glBindTexture(GL_TEXTURE_2D, colorbuffer);                     \
        glActiveTexture(GL_TEXTURE1);                                  \
        glBindTexture(GL_TEXTURE_2D, depthbuffer);                     \
    }

namespace Render {

Renderer::Renderer(QWidget *parent) : QOpenGLWidget(parent) {
    // 初始化顶点数据
    terrainMesh = nullptr;
    terrainVBO = nullptr;
    terrainEBO = nullptr;
    terrainVAO = nullptr;
    screenVBO = nullptr;
    screenVAO = nullptr;

    // 初始化数据组
    panelVertices = nullptr;
    panelIndices = nullptr;
    rectVertices = nullptr;

    // 初始化渲染shader
    terrainVert = nullptr;
    terrainFrag_DirectLight = nullptr;
    terrainFrag_WorldNormal = nullptr;
    terrainFrag_WorldPos = nullptr;

    terrainShaderProgram_DirectLight = nullptr;
    terrainShaderProgram_WorldPos = nullptr;
    terrainShaderProgram_WorldNorm = nullptr;
    terrainShaderProgram_Albedo = nullptr;

    terrainShader_RayTracing = nullptr;
    terrainShaderProgram_RayTracing = nullptr;

    // 初始化离屏渲染交换链
    swapVert = nullptr;
    swapFrag = nullptr;

    G_Buffer_DirectLight = new RenderTexture(this, true);
    G_Buffer_WorldPos = new RenderTexture(this, true);
    G_Buffer_WorldNormal = new RenderTexture(this, true);
    G_Buffer_Albedo = new RenderTexture(this, false);

    ANS_Buffer_RayTracing = new RenderImage2D(this);

    //    swapFrameBuffer = 0;
    //    swapColorBuffer = 0;
    //    swapDepthBuffer = 0;

    // 初始化摄像机矩阵
    camera = new Camera(this, 16.0 / 9.0, 25);
    model = QMatrix4x4();
    view = QMatrix4x4();
    proj = QMatrix4x4();
    isControlling = false;
    camCtrlMode = CameraControlMode::None;
    azimuth = 0;
    altitude = M_PI / 4.0;
    distance = sqrt(2.0);
    rotateSensitive = 0.005f;
    scaleSensitive = 0.001f;

    // 启动计时器
    timer = new QTimer(this);
    timer->setTimerType(Qt::PreciseTimer);
    timer->setInterval(33);
    timer->start();
    // 根据计时器信号更新画面
    connect(timer, SIGNAL(timeout()), this, SLOT(TimerUpdate()));
}

void Renderer::TimerUpdate() {
    static unsigned int timerNum = 0;
    if (timerNum < 250) timerNum++;

    static unsigned int oldHeightFieldBufferIndex =
        globalinfo::ChosenHeightFieldBuffer;
    if (oldHeightFieldBufferIndex != globalinfo::ChosenHeightFieldBuffer) {
        update();
        oldHeightFieldBufferIndex = globalinfo::ChosenHeightFieldBuffer;
    }
    if (globalrender::real_time_render) {
        update();
    }
}

void Renderer::initializeGL() {
    // 首先初始化OpenGL函数
    initializeOpenGLFunctions();

    // 分配内存
    // VBO/EBO/VAO在窗口运行时千万不能delete
    terrainVBO = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    terrainEBO = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
    terrainVAO = new QOpenGLVertexArrayObject();

    terrainVBO->create();
    terrainEBO->create();
    terrainVAO->create();

    // 根据设置数据生成顶点数据、vbo、vao、ebo
    //    setTerrainInfo();

    terrainMesh = new TerrainMesh(this);
    terrainMesh->recreateMesh(*(getFunctionAndContext()));

    qDebug() << "初始化错误验证 "
             << "根据设置数据生成顶点数据、vbo、vao、ebo，错误码："
             << glGetError();

    // 渲染shader内存分配
    terrainVert = new QOpenGLShader(QOpenGLShader::Vertex);
    terrainFrag_DirectLight = new QOpenGLShader(QOpenGLShader::Fragment);
    terrainFrag_WorldPos = new QOpenGLShader(QOpenGLShader::Fragment);
    terrainFrag_WorldNormal = new QOpenGLShader(QOpenGLShader::Fragment);

    terrainShaderProgram_DirectLight = new QOpenGLShaderProgram();
    terrainShaderProgram_DirectLight->create();
    terrainShaderProgram_WorldNorm = new QOpenGLShaderProgram();
    terrainShaderProgram_WorldNorm->create();
    terrainShaderProgram_WorldPos = new QOpenGLShaderProgram();
    terrainShaderProgram_WorldPos->create();

    terrainShader_RayTracing = new QOpenGLShader(QOpenGLShader::Compute);
    terrainShader_RayTracing->compileSourceFile(
        ":/TerrainShaders/RayTracing01.comp");

    terrainShaderProgram_RayTracing = new QOpenGLShaderProgram();
    terrainShaderProgram_RayTracing->create();
    terrainShaderProgram_RayTracing->addShader(terrainShader_RayTracing);
    terrainShaderProgram_RayTracing->link();

    // 渲染shader读取
    setRenderShaders();

    qDebug() << "初始化错误验证 "
             << "渲染shader，错误码：" << glGetError();

    // 离屏渲染shader准备
    swapVert = new QOpenGLShader(QOpenGLShader::Vertex);
    swapFrag = new QOpenGLShader(QOpenGLShader::Fragment);

    swapShaderProgram = new QOpenGLShaderProgram();
    swapShaderProgram->create();

    // 离屏渲染buffer准备
    screenVBO = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    screenVAO = new QOpenGLVertexArrayObject();
    screenVBO->create();
    screenVAO->create();

    // RT创建
    G_Buffer_DirectLight->recreateRenderTexture(rect().width(), rect().height(),
                                                *(getFunctionAndContext()));
    G_Buffer_WorldPos->recreateRenderTexture(rect().width(), rect().height(),
                                             *(getFunctionAndContext()));
    G_Buffer_WorldNormal->recreateRenderTexture(rect().width(), rect().height(),
                                                *(getFunctionAndContext()));
    G_Buffer_Albedo->recreateRenderTexture(rect().width(), rect().height(),
                                           *(getFunctionAndContext()));

    ANS_Buffer_RayTracing->recreateRenderImage2D(
        rect().width(), rect().height(), *(getFunctionAndContext()));

    // 离屏渲染（交换链）所有内容准备
    prepareSwapChain();

    qDebug() << "初始化错误验证 "
             << "离屏渲染所有内容准备，错误码：" << glGetError();

    // 鼠标控制变量准备
    mouseOldPos = QPoint();

    // 摄像机准备
    //    camPos = QVector3D(0.0, 1.0, -1.0);
    //    setCameraInfo();
    // 物体矩阵准备
    model.setToIdentity();
    model.scale(0.001f);

    // 场景主光源准备
    sunLight.setColor(QColor(255, 245, 245));
    sunLight.setStrength(15);
    sunLight.setDirection(QVector3D(1.5, 1.2, 0.7));

    qDebug() << "初始化错误验证 "
             << "摄像机内容准备，错误码：" << glGetError();
}

void Renderer::resizeGL(int w, int h) {
    makeCurrent();
    // 重设texture
    //    switchFrameBufferAndRenderTextures();
    G_Buffer_DirectLight->recreateRenderTexture(w, h,
                                                *(getFunctionAndContext()));
    G_Buffer_WorldPos->recreateRenderTexture(rect().width(), rect().height(),
                                             *(getFunctionAndContext()));
    G_Buffer_WorldNormal->recreateRenderTexture(rect().width(), rect().height(),
                                                *(getFunctionAndContext()));
    G_Buffer_Albedo->recreateRenderTexture(rect().width(), rect().height(),
                                           *(getFunctionAndContext()));
    ANS_Buffer_RayTracing->recreateRenderImage2D(w, h + 50,
                                                 *(getFunctionAndContext()));
    // 参数X，Y指定了视见区域的左下角在窗口中的位置，一般情况下为（0，0），Width和Height指定了视见区域的宽度和高度。
    //    int ratio =
    //    QApplication::desktop()->devicePixelRatio();
    //    proj.setToIdentity();
    //    proj.perspective(45, GLfloat(w) / h, 0.01, 15.0);

    camera->setAspectRatio(GLfloat(w) / h);
    //    proj.perspective(45, 1, 0.01, 15.0);
    //    setCameraInfo(w, h);
    update();
}

void Renderer::paintGL() {
    // 同步上下文
    globalgl::thisContext = getFunctionAndContext();

    // 准备离屏渲染
    //    PreRenderTerrainGround(swapFrameBuffer);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glBindImageTexture(0, globalinfo::ChosenHeightFieldBuffer, 0, GL_FALSE, 0,
                       GL_READ_WRITE, GL_R32F);
    glBindImageTexture(1, globalinfo::ColorMap0, 0, GL_FALSE, 0, GL_READ_WRITE,
                       GL_RGBA32F);

    G_Buffer_DirectLight->clear(*(getFunctionAndContext()));
    G_Buffer_DirectLight->bind(*(getFunctionAndContext()));
    bindShaderAndPassParam(*terrainShaderProgram_DirectLight);
    terrainMesh->drawMesh(*(getFunctionAndContext()));

    G_Buffer_WorldNormal->clear(*(getFunctionAndContext()));
    G_Buffer_WorldNormal->bind(*(getFunctionAndContext()));
    bindShaderAndPassParam(*terrainShaderProgram_WorldNorm);
    terrainMesh->drawMesh(*(getFunctionAndContext()));

    G_Buffer_WorldPos->clear(*(getFunctionAndContext()));
    G_Buffer_WorldPos->bind(*(getFunctionAndContext()));
    bindShaderAndPassParam(*terrainShaderProgram_WorldPos);
    terrainMesh->drawMesh(*(getFunctionAndContext()));
    //    terrainVAO->bind();
    //    glDrawElements(GL_TRIANGLES, indicesCount(), GL_UNSIGNED_INT, 0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, G_Buffer_DirectLight->colorTexture());
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, G_Buffer_WorldPos->colorTexture());
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, G_Buffer_WorldNormal->colorTexture());

    glActiveTexture(GL_TEXTURE0);

    // 光线追踪阶段
    terrainShaderProgram_RayTracing->bind();

    terrainShaderProgram_RayTracing->setUniformValue("RTColor", 0);
    terrainShaderProgram_RayTracing->setUniformValue("RTWorldPos", 1);
    terrainShaderProgram_RayTracing->setUniformValue("RTNormal", 2);

    terrainShaderProgram_RayTracing->setUniformValue("model", model);
    terrainShaderProgram_RayTracing->setUniformValue(
        "view_inverse", camera->matrixView().inverted());
    terrainShaderProgram_RayTracing->setUniformValue(
        "proj_inverse", camera->matrixProjection().inverted());
    terrainShaderProgram_RayTracing->setUniformValue("near", camera->Near());
    terrainShaderProgram_RayTracing->setUniformValue("far", camera->Far());
    terrainShaderProgram_RayTracing->setUniformValue("cameraPos",
                                                     camera->cameraPos());

    terrainShaderProgram_RayTracing->setUniformValue("screenWidth",
                                                     rect().width());
    terrainShaderProgram_RayTracing->setUniformValue("screenHeight",
                                                     rect().height());
    terrainShaderProgram_RayTracing->setUniformValue("TerrainGrid",
                                                     globalinfo::TerrainGrid);
    terrainShaderProgram_RayTracing->setUniformValue("TerrainSize",
                                                     globalinfo::TerrainSize);
    terrainShaderProgram_RayTracing->setUniformValue("TerrainHeight",
                                                     globalinfo::TerrainHeight);

    terrainShaderProgram_RayTracing->setUniformValue("mainLightColor",
                                                     sunLight.getColor());
    terrainShaderProgram_RayTracing->setUniformValue("mainLightStrength",
                                                     sunLight.getStrength());
    terrainShaderProgram_RayTracing->setUniformValue("mainLightDir",
                                                     sunLight.getDirection());

    QTime currentTime = QTime::currentTime();
    terrainShaderProgram_RayTracing->setUniformValue("time.hour",
                                                     currentTime.hour());
    terrainShaderProgram_RayTracing->setUniformValue("time.minute",
                                                     currentTime.minute());
    terrainShaderProgram_RayTracing->setUniformValue("time.second",
                                                     currentTime.second());
    terrainShaderProgram_RayTracing->setUniformValue("time.msec",
                                                     currentTime.msec());

    ANS_Buffer_RayTracing->bind(0, *(getFunctionAndContext()));
    glBindImageTexture(1, globalinfo::ChosenHeightFieldBuffer, 0, GL_FALSE, 0,
                       GL_READ_WRITE, GL_R32F);

    glDispatchCompute(int(rect().width() / 32) + 1, int(rect().height() / 32),
                      1);

    // 帧缓冲复制阶段
    PreSwapFrameBufferToScreen(G_Buffer_DirectLight->colorTexture(),
                               G_Buffer_DirectLight->depthTexture());

    swapShaderProgram->bind();
    swapShaderProgram->setUniformValue("colorBuffer", 0);
    swapShaderProgram->setUniformValue("depthBuffer", 1);
    swapShaderProgram->setUniformValue("screenWidth", rect().width());
    swapShaderProgram->setUniformValue("screenHeight", rect().height());
    screenVAO->bind();
    glDrawArrays(GL_TRIANGLES, 0, 6);

    swapShaderProgram->release();
}

void Renderer::setTerrainInfo() {
    if (panelVertices != nullptr) delete[] panelVertices;
    panelVertices = nullptr;
    if (panelIndices != nullptr) delete[] panelIndices;
    panelIndices = nullptr;

    panelVertices = new float[verticiesCount()];
    panelIndices = new unsigned int[indicesCount()];

    generateMesh();
    qDebug() << "初始化错误验证 "
             << "网格顶点数组在RAM中准备，错误码：" << glGetError();

    allocateBuffer();
    qDebug() << "初始化错误验证 "
             << "生成VBO、VAO，错误码：" << glGetError();
}

void Renderer::generateMesh() {
    float unit = ((float)globalinfo::TerrainSize) / globalinfo::TerrainGrid;
    float offset = ((float)globalinfo::TerrainSize) / 2;
    for (int z = 0; z < RowVertsNumber(); z++) {
        for (int x = 0; x < RowVertsNumber(); x++) {
            int index = (z * RowVertsNumber() + x) * 2;
            panelVertices[index + 0] = (x - 1) * unit - offset;
            panelVertices[index + 1] = (z - 1) * unit - offset;
        }
    }

    for (int z = 0; z < RowRectsNumber(); z++) {
        for (int x = 0; x < RowRectsNumber(); x++) {
            int iindex = (z * RowRectsNumber() + x) * 6;
            int vindex = z * RowVertsNumber() + x;
            panelIndices[iindex + 0] = vindex + 0;
            panelIndices[iindex + 1] = vindex + 1;
            panelIndices[iindex + 2] = vindex + RowVertsNumber() + 1;
            panelIndices[iindex + 3] = vindex + 0;
            panelIndices[iindex + 4] = vindex + RowVertsNumber() + 1;
            panelIndices[iindex + 5] = vindex + RowVertsNumber();
        }
    }
    //    for (uint i = 0; i < verticiesCount(); i++) {
    //        std::cout << panelVertices[i] << " ";
    //        if (i % 30 == 0) std::cout << "\n";
    //    }
}

unsigned int Renderer::verticiesCount() const {
    return RowVertsNumber() * RowVertsNumber() * 2;
}

unsigned int Renderer::indicesCount() const {
    return RowRectsNumber() * RowRectsNumber() * 6;
}

void Renderer::allocateBuffer() {
    bool flag = true;
    if (terrainVBO == nullptr || !(terrainVBO->isCreated())) {
        flag = false;
        qDebug() << "ERROR: VBO object is not created or unable!";
    }
    if (terrainVAO == nullptr || !(terrainVAO->isCreated())) {
        flag = false;
        qDebug() << "ERROR: VAO object is not created or unable!";
    }
    if (terrainEBO == nullptr || !(terrainEBO->isCreated())) {
        flag = false;
        qDebug() << "ERROR: EBO object is not created or unable!";
    }
    if (!flag) {
        qDebug() << "Cannot allocate buffers!";
        return;
    }

    // 绑定VAO和VBO，分配顶点
    terrainVAO->bind();
    terrainVBO->bind();
    terrainVBO->allocate(panelVertices, sizeof(float) * verticiesCount());
    // 绑定EBO，分配索引
    terrainEBO->bind();
    terrainEBO->allocate(panelIndices, sizeof(unsigned int) * indicesCount());

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float),
                          (void *)0);
    glEnableVertexAttribArray(0);
}

void Renderer::prepareSwapChain() {
    // 屏幕空间矩形顶点数据
    rectVertices = new float[12]{-1, -1, 1, -1, 1, 1, -1, -1, 1, 1, -1, 1};

    // 屏幕vbo、vao生成
    screenVAO->bind();
    screenVBO->bind();
    screenVBO->allocate(rectVertices, sizeof(float) * 12);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float),
                          (void *)0);
    glEnableVertexAttribArray(0);

    //    createFrameBufferAndRenderTextures();

    // 离屏渲染交换链shader读取
    bool flag = true;
    if (swapVert == nullptr) {
        qDebug() << "ERROR: Swap chain vertex shader is not "
                    "created!";
        flag = false;
    }
    if (swapFrag == nullptr) {
        qDebug() << "ERROR: Swap chain frag shader is not created!";
        flag = false;
    }
    if (swapShaderProgram == nullptr) {
        qDebug() << "ERROR: Swap chain shader program is not "
                    "created!";
        flag = false;
    }
    if (!flag) {
        qDebug() << "Cannot set swap screen shaders.";
        return;
    }
    swapVert->compileSourceFile(":/SwapScreenShaders/ScreenVert.vert");
    swapFrag->compileSourceFile(":/SwapScreenShaders/ScreenFrag.frag");

    swapShaderProgram->addShader(swapVert);
    swapShaderProgram->addShader(swapFrag);
    swapShaderProgram->link();
}

void Renderer::createFrameBufferAndRenderTextures() {
    // 生成frame buffer
    glGenFramebuffers(1, &swapFrameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, swapFrameBuffer);

    // 生成第一张color texture
    glGenTextures(1, &swapColorBuffer);
    glBindTexture(GL_TEXTURE_2D, swapColorBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width(), height(), 0, GL_RGB,
                 GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                           swapColorBuffer, 0);

    // 生成第二场depth buffer
    glGenTextures(1, &swapDepthBuffer);
    glBindTexture(GL_TEXTURE_2D, swapDepthBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, width(), height(), 0,
                 GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glBindTexture(GL_TEXTURE_2D, 0);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,
                           swapDepthBuffer, 0);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        qDebug() << "ERROR::FRAMEBUFFER:: Framebuffer is not "
                    "complete!";
    glBindFramebuffer(GL_FRAMEBUFFER, defaultFramebufferObject());
}

void Renderer::switchFrameBufferAndRenderTextures() {
    // 绑定fbo
    glBindFramebuffer(GL_FRAMEBUFFER, swapFrameBuffer);
    // 解绑texture
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                           0, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,
                           0, 0);

    // 删除texture
    glDeleteTextures(1, &swapColorBuffer);
    glDeleteTextures(1, &swapDepthBuffer);

    // 重新分配texture
    // 生成第一张color texture
    glGenTextures(1, &swapColorBuffer);
    glBindTexture(GL_TEXTURE_2D, swapColorBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width(), height(), 0, GL_RGB,
                 GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                           swapColorBuffer, 0);

    // 生成第二场depth buffer
    glGenTextures(1, &swapDepthBuffer);
    glBindTexture(GL_TEXTURE_2D, swapDepthBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, width(), height(), 0,
                 GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glBindTexture(GL_TEXTURE_2D, 0);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,
                           swapDepthBuffer, 0);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        qDebug() << "ERROR::FRAMEBUFFER:: Framebuffer is not "
                    "complete!";
    glBindFramebuffer(GL_FRAMEBUFFER, defaultFramebufferObject());
}

void Renderer::setRenderShaders() {
    bool flag = true;
    if (terrainVert == nullptr) {
        qDebug() << "ERROR: Vertex shader is not created!";
        flag = false;
    }
    if (terrainFrag_DirectLight == nullptr) {
        qDebug() << "ERROR: Frag shader is not created!";
        flag = false;
    }
    if (terrainShaderProgram_DirectLight == nullptr) {
        qDebug() << "ERROR: Shader program is not created!";
        flag = false;
    }
    if (!flag) {
        qDebug() << "Cannot set render shaders.";
        return;
    }
    terrainVert->compileSourceFile(":/TerrainShaders/TerrainVert.vert");
    terrainFrag_DirectLight->compileSourceFile(
        ":/TerrainShaders/TerrainFrag.frag");

    terrainShaderProgram_DirectLight->addShader(terrainVert);
    terrainShaderProgram_DirectLight->addShader(terrainFrag_DirectLight);
    terrainShaderProgram_DirectLight->link();

    terrainFrag_WorldPos->compileSourceFile(
        ":/TerrainShaders/TerrainFrag_WorldPos.frag");
    terrainFrag_WorldNormal->compileSourceFile(
        ":/TerrainShaders/TerrainFrag_WorldNormal.frag");

    terrainShaderProgram_WorldPos->addShader(terrainVert);
    terrainShaderProgram_WorldPos->addShader(terrainFrag_WorldPos);
    terrainShaderProgram_WorldPos->link();

    terrainShaderProgram_WorldNorm->addShader(terrainVert);
    terrainShaderProgram_WorldNorm->addShader(terrainFrag_WorldNormal);
    terrainShaderProgram_WorldNorm->link();
}

void Renderer::bindShaderAndPassParam(QOpenGLShaderProgram &shader) {
    shader.bind();
    shader.setUniformValue("ColorMap0_value", globalinfo::ColorMap0);

    shader.setUniformValue("model", model);
    shader.setUniformValue("view", camera->matrixView());
    shader.setUniformValue("proj", camera->matrixProjection());
    shader.setUniformValue("view_inverse", camera->matrixView().inverted());
    shader.setUniformValue("proj_inverse",
                           camera->matrixProjection().inverted());

    shader.setUniformValue("TerrainHeight", globalinfo::TerrainHeight);
    shader.setUniformValue("TerrainSize", globalinfo::TerrainSize);
    shader.setUniformValue("TerrainGrid", globalinfo::TerrainGrid);
    shader.setUniformValue("useHeightFieldBuffer",
                           globalinfo::useHeightFieldBuffer);

    shader.setUniformValue("mainLightColor", sunLight.getColor());
    shader.setUniformValue("mainLightStrength", sunLight.getStrength());
    shader.setUniformValue("mainLightDir", sunLight.getDirection());
}

void Renderer::setCameraInfo() {
    //    camPos = QVector3D(distance * sinf(altitude) * cosf(azimuth),
    //                       distance * cosf(altitude),
    //                       distance * sinf(altitude) * sinf(azimuth));

    model.setToIdentity();
    model.scale(0.001f);
    //    view.setToIdentity();
    //    view.lookAt(camPos, QVector3D(0, 0, 0), QVector3D(0, 1, 0));
    //    proj.setToIdentity();
    //    proj.perspective(45, ((float)width()) / ((float)height()),
    //    0.01, 15.0);
}

void Renderer::setCameraInfo(int w, int h) {
    camPos = QVector3D(distance * sinf(altitude) * cosf(azimuth),
                       distance * cosf(altitude),
                       distance * sinf(altitude) * sinf(azimuth));

    model.setToIdentity();
    model.scale(0.001f);
    //    view.setToIdentity();
    //    view.lookAt(camPos, QVector3D(0, 0, 0), QVector3D(0, 1, 0));
    //    proj.setToIdentity();
    //    proj.perspective(45, ((float)w) / ((float)h), 0.01, 15.0);
}

void Renderer::mousePressEvent(QMouseEvent *event) {
    isControlling = true;
    mouseOldPos = event->pos();
    if (event->button() == Qt::LeftButton) {
        camCtrlMode = CameraControlMode::Rotate;
    } else {
        camCtrlMode = CameraControlMode::Translate;
    }
}

void Renderer::mouseMoveEvent(QMouseEvent *event) {
    QPoint deltaP = event->pos() - mouseOldPos;
    //    azimuth += deltaP.x() * rotateSensitive;
    //    altitude -= deltaP.y() * rotateSensitive;
    //    if (altitude >= M_PI / 2)
    //        altitude = M_PI / 2;
    //    else if (altitude <= 0.01)
    //        altitude = 0.01;
    //    setCameraInfo();
    if (camCtrlMode == CameraControlMode::Rotate)
        camera->rotate(deltaP.x(), deltaP.y());
    else if (camCtrlMode == CameraControlMode::Translate)
        camera->translate(deltaP.x(), deltaP.y());
    mouseOldPos = event->pos();
    update();
}

void Renderer::mouseReleaseEvent(QMouseEvent *event) {
    mouseMoveEvent(event);
    isControlling = false;
    camCtrlMode = CameraControlMode::None;
}

void Renderer::wheelEvent(QWheelEvent *event) {
    //    distance *= exp(-(event->angleDelta().x() + event->angleDelta().y()) *
    //                    scaleSensitive);

    int delta = event->delta();
    if (delta > 0) {
        camera->zoomin();
    } else if (delta < 0) {
        camera->zoomout();
    }

    //    setCameraInfo();
    update();
}

QOpenGLFunctions_4_5_Core *Renderer::getFunctionAndContext() {
    QOpenGLFunctions_4_5_Core *f = this;
    return f;
}

} // namespace Render
