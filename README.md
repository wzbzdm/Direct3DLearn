# DirectXD 3D图形渲染框架

## 项目概述
DirectXD是一个基于DirectX的3D图形渲染框架，提供多种3D图形渲染和交互功能。

## 主要特性
- 支持多种3D图形：立方体(Hexahedron)、圆柱体(Cylinder)、球体(Sphere)等
- 使用DirectX 11渲染管线
- 多线程事件处理机制
- 完整的鼠标交互支持(点击、移动、滚轮等)
- 内置计时器和窗口管理

## 技术栈
- C++17
- DirectX 11
- HLSL着色器
- 多线程编程(std::thread, std::mutex)

## 项目结构
```
DirectXD/
├── 3D图形类/
│   ├── Hexahedron3D.h/cpp    # 立方体
│   ├── Cylinder3D.h/cpp      # 圆柱体 
│   ├── Sphere3D.h/cpp        # 球体
│   └── ...
├── 着色器/
│   ├── BoxVertexShader.hlsl  # 立方体顶点着色器
│   ├── LightPixelShader.hlsl # 光照像素着色器
│   └── ...
├── 核心组件/
│   ├── SimpleDraw.h/cpp       # 主渲染循环
│   ├── MyWindow.h/cpp        # 窗口管理
│   ├── MyTimer.h/cpp         # 计时器
│   └── ...
└── ...
```

## 构建说明
1. 确保安装Visual Studio 2019或更高版本
2. 安装DirectX SDK
3. 打开DirectXD.sln解决方案文件
4. 编译运行

## 许可证
MIT License
