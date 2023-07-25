#include "renderer.h"

#define PreRenderTerrainGround(framebuffer)                 \
    {                                                       \
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);     \
        glClearColor(0.2f, 0.8f, 1, 1);                     \
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); \
        glEnable(GL_DEPTH_TEST);                            \
        glDisable(GL_BLEND);                                \
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

Renderer::Renderer(QWidget *parent) : QOpenGLWidget(parent) {
    // 初始化顶点数据
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
    terrainFrag = nullptr;

    // 初始化离屏渲染交换链
    swapVert = nullptr;
    swapFrag = nullptr;

    swapFrameBuffer = 0;
    swapColorBuffer = 0;
    swapDepthBuffer = 0;

    // 初始化摄像机矩阵
    model = QMatrix4x4();
    view = QMatrix4x4();
    proj = QMatrix4x4();
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
    setTerrainInfo();

    // 渲染shader内存分配
    terrainVert = new QOpenGLShader(QOpenGLShader::Vertex);
    terrainFrag = new QOpenGLShader(QOpenGLShader::Fragment);

    terrainShaderProgram = new QOpenGLShaderProgram();
    terrainShaderProgram->create();

    // 渲染shader读取
    setRenderShaders();

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

    // 离屏渲染（交换链）所有内容准备
    prepareSwapChain();

    // 摄像机准备
    setCameraInfo();

    ChosenHeightFieldBuffer = 0;

    qDebug() << glGetError();
}

void Renderer::resizeGL(int w, int h) {
    setCameraInfo();
}

void Renderer::paintGL() {
    PreRenderTerrainGround(swapFrameBuffer);

    terrainShaderProgram->bind();
    glBindImageTexture(0, ChosenHeightFieldBuffer, 0, GL_FALSE, 0,
                       GL_READ_WRITE, GL_RGBA32F);
    qDebug() << "test" << ChosenHeightFieldBuffer;
    terrainShaderProgram->setUniformValue("model", model);
    terrainShaderProgram->setUniformValue("view", view);
    terrainShaderProgram->setUniformValue("proj", proj);
    terrainShaderProgram->setUniformValue("TerrainHeight",
                                          globalinfo::TerrainHeight);
    terrainShaderProgram->setUniformValue("TerrainSize",
                                          globalinfo::TerrainSize);
    terrainShaderProgram->setUniformValue("TerrainGrid",
                                          globalinfo::TerrainGrid);
    terrainShaderProgram->setUniformValue("useHeightFieldBuffer",
                                          globalinfo::useHeightFieldBuffer);

    terrainVAO->bind();
    glDrawElements(GL_TRIANGLES, indicesCount(), GL_UNSIGNED_INT, 0);

    qDebug() << "场景渲染后" << glGetError();

    PreSwapFrameBufferToScreen(swapColorBuffer, swapDepthBuffer);

    swapShaderProgram->bind();
    swapShaderProgram->setUniformValue("colorBuffer", 0);
    swapShaderProgram->setUniformValue("depthBuffer", 1);
    screenVAO->bind();
    glDrawArrays(GL_TRIANGLES, 0, 6);
    qDebug() << "交换渲染后" << glGetError();
}

void Renderer::setTerrainInfo() {
    if (panelVertices != nullptr) delete[] panelVertices;
    panelVertices = nullptr;
    if (panelIndices != nullptr) delete[] panelIndices;
    panelIndices = nullptr;

    panelVertices = new float[verticiesCount()];
    panelIndices = new unsigned int[indicesCount()];

    generateMesh();
    allocateBuffer();
}

void Renderer::generateMesh() {
    float unit = ((float)globalinfo::TerrainSize) / globalinfo::TerrainGrid;
    float offset = ((float)globalinfo::TerrainSize) / 2;
    for (int z = 0; z < globalinfo::TerrainGrid; z++) {
        for (int x = 0; x < globalinfo::TerrainGrid; x++) {
            int index = (z * globalinfo::TerrainGrid + x) * 2;
            panelVertices[index + 0] = x * unit - offset;
            panelVertices[index + 1] = z * unit - offset;
        }
    }

    for (int z = 0; z < globalinfo::TerrainGrid - 1; z++) {
        for (int x = 0; x < globalinfo::TerrainGrid - 1; x++) {
            int iindex = (z * (globalinfo::TerrainGrid - 1) + x) * 6;
            int vindex = z * globalinfo::TerrainGrid + x;
            panelIndices[iindex + 0] = vindex + 0;
            panelIndices[iindex + 1] = vindex + 1;
            panelIndices[iindex + 2] = vindex + globalinfo::TerrainGrid + 1;
            panelIndices[iindex + 3] = vindex + 0;
            panelIndices[iindex + 4] = vindex + globalinfo::TerrainGrid + 1;
            panelIndices[iindex + 5] = vindex + globalinfo::TerrainGrid;
        }
    }
    //    for (uint i = 0; i < verticiesCount(); i++) {
    //        std::cout << panelVertices[i] << " ";
    //        if (i % 30 == 0) std::cout << "\n";
    //    }
}

unsigned int Renderer::verticiesCount() const {
    return globalinfo::TerrainGrid * globalinfo::TerrainGrid * 2;
}

unsigned int Renderer::indicesCount() const {
    return (globalinfo::TerrainGrid - 1) * (globalinfo::TerrainGrid - 1) * 6;
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
        qDebug() << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!";
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // 离屏渲染交换链shader读取
    bool flag = true;
    if (swapVert == nullptr) {
        qDebug() << "ERROR: Swap chain vertex shader is not created!";
        flag = false;
    }
    if (swapFrag == nullptr) {
        qDebug() << "ERROR: Swap chain frag shader is not created!";
        flag = false;
    }
    if (swapShaderProgram == nullptr) {
        qDebug() << "ERROR: Swap chain shader program is not created!";
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

void Renderer::setRenderShaders() {
    bool flag = true;
    if (terrainVert == nullptr) {
        qDebug() << "ERROR: Vertex shader is not created!";
        flag = false;
    }
    if (terrainFrag == nullptr) {
        qDebug() << "ERROR: Frag shader is not created!";
        flag = false;
    }
    if (terrainShaderProgram == nullptr) {
        qDebug() << "ERROR: Shader program is not created!";
        flag = false;
    }
    if (!flag) {
        qDebug() << "Cannot set render shaders.";
        return;
    }
    terrainVert->compileSourceFile(":/TerrainShaders/TerrainVert.vert");
    terrainFrag->compileSourceFile(":/TerrainShaders/TerrainFrag.frag");

    terrainShaderProgram->addShader(terrainVert);
    terrainShaderProgram->addShader(terrainFrag);
    terrainShaderProgram->link();
}

void Renderer::setCameraInfo() {
    model = QMatrix4x4();
    model.scale(0.001f);
    view = QMatrix4x4();
    view.lookAt(QVector3D(0.0, 1.0, -1.0), QVector3D(0, 0, 0),
                QVector3D(0, 1, 0));
    proj = QMatrix4x4();
    proj.perspective(45, ((float)width()) / height(), 0.01, 15.0);
}

QOpenGLFunctions_4_5_Core *Renderer::getFunctionAndContext() {
    QOpenGLFunctions_4_5_Core *f = this;
    return f;
}
