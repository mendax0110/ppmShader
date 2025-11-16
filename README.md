# ppmShader - A Simple PPM Image Shader

ppmShader is a lightweight and easy-to-use shader for rendering PPM (Portable Pixmap) images in graphics applications. It supports basic shading techniques and is designed for educational purposes and simple graphics projects.
Inspired from a short video of Tsoding, explaining how to write a shader with ppm files and then pipe it into ffmpeg to create videos.

## Build
To build ppmShader, you need to have a C++ compiler installed. You can use the following command to compile the shader:

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

## Usage
To use ppmShader, you need to pipe the output into ffmpeg to create videos. Here is an example command:

```bash
./ppmShader 640 360 300 30 | ffmpeg -f image2pipe -vcodec ppm -r 30 -i - -pix_fmt yuv420p tokamak.mp4
./ppmShader 640 360 300 30 helix | ffmpeg -f image2pipe -vcodec ppm -r 30 -i - -pix_fmt yuv420p helix.mp4

640 = width
360 = height
300 = number of frames
30 = frames per second
```

## Videos Created with ppmShader

Here are some example videos created using ppmShader:
- [Tokamak](videos/tokamak.mp4)
- [Helix](videos/helix.mp4)


