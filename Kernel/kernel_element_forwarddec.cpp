// 用于前置声明
#define AllForwardDeclaration \
    class Port;               \
    class Wire;               \
    class Node;               \
    class Clamp_Node;         \
    class PerlinNoise_Node;   \
    class NodeGraph;

#define ForwardDeclarationWithoutGraph \
    class Port;                        \
    class Wire;                        \
    class Node;                        \
    class Clamp_Node;                  \
    class PerlinNoise_Node;

#define ForwardDeclarationWithoutPort \
    class Wire;                       \
    class Node;                       \
    class Clamp_Node;                 \
    class PerlinNoise_Node;           \
    class NodeGraph;

#define ForwardDeclarationWithoutWire \
    class Port;                       \
    class Node;                       \
    class Clamp_Node;                 \
    class PerlinNoise_Node;           \
    class NodeGraph;
