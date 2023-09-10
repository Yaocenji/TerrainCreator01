#version 450 core 

#define PI 3.14159265

layout(location = 0)in vec2 pos;

layout (binding = 0, rgba32f) uniform image2D HeightField;

out vec4 worldPos;
out vec2 texCoord;
out vec4 clipPos;
out vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

uniform int TerrainGrid;
uniform float TerrainSize;
uniform float TerrainHeight;

uniform sampler2D TerrainData;

uniform bool useHeightFieldBuffer;


// 获取高度场数据
// param0:给出高度长
// param1:坐标值（原始索引）
float GetHeightData(int x, int y){
    float ans = imageLoad(HeightField, ivec2(x, y)).x;
    return ans;
}
// 获取高度场一处数据
// param0:给出高度长
// param1:坐标值（原始索引）
// param2:要设置的值
void SetHeightData(int x, int y, float value){
    imageStore(HeightField, ivec2(x, y), vec4(value, value, value, value));
}

// 计算梯度：
// param0:给出高度场
// param1:高度场的细分网格数
// param2:高度场的真实大小
// param3:规范化到0-1的位置坐标值
vec2 CalGrad(int Grid, float Size, float x, float y)
{
    // 计算梯度
    // 还原坐标的小数部分
    float u = fract(x * Grid);
    float v = fract(y * Grid);
    // 还原坐标的整数部分
    int xInt = int(x * Grid);
    int yInt = int(y * Grid);
    int xplus = xInt < Grid - 1 ? xInt + 1 : xInt;
    int yplus = yInt < Grid - 1 ? yInt + 1 : yInt;

    vec2 grad;
    grad.x = 
        (GetHeightData(xplus, yInt) - GetHeightData(xInt, yInt)) * (1.0 - v) 
        + (GetHeightData(xplus, yplus) - GetHeightData(xInt, yplus)) * v;
    grad.y = 
        (GetHeightData(xInt, yplus) - GetHeightData(xInt, yInt)) * (1.0 - u) 
        + (GetHeightData(xplus, yplus) - GetHeightData(xplus, yInt)) * u;
    grad /= Size * 2.0 / float(Grid);

    return grad;
}

// 从梯度计算法线
vec3 Grad2Norm(vec2 grad){
    // 斜率绝对值
    float abs_k = length(grad);
    // 法线向量
    vec3 normal = normalize(normalize(vec3(grad.x, 0, grad.y)) + vec3(0, 1, 0) / abs_k);
    return normal;
}

// 计算法线
// param0:给出高度场
// param1:高度场的细分网格数
// param2:高度场的真实大小
// param3:规范化到0-1的位置坐标值
vec3 CalNorm(int Grid, float Size, float x, float y)
{
    vec2 grad = CalGrad(Grid, Size, x, y);
    vec3 norm = Grad2Norm(grad);
    return norm;
}


void main(){
    vec2 rtexCoord = 2.0 * pos.xy / TerrainSize;
    texCoord = pos.xy / TerrainSize + vec2(0.5, 0.5); 
    ivec2 itexCoord = ivec2(texCoord.xy * TerrainGrid);
    // 根据传入顶点信息构建三维顶点
    vec4 modelPos;
    if (useHeightFieldBuffer){
        float HeightFieldData = imageLoad(HeightField, itexCoord).r;
        modelPos = vec4(pos.x, HeightFieldData, pos.y, 1.0);
        normal = CalNorm(TerrainGrid, TerrainSize, texCoord.x, texCoord.y);
    }
    else{
        float dist = length(rtexCoord);
        dist *= 3;
        float HeightData = 2 * TerrainHeight * 1.0 / sqrt(2.0 * PI) * exp(- dist * dist / 2.0);
        modelPos = vec4(pos.x, HeightData, pos.y, 1.0);
        normal = vec3(0, 1, 0);
    }
    // 乘以mvp矩阵
    worldPos = model * modelPos;
    clipPos = proj * view * worldPos;
    gl_Position = clipPos;
}