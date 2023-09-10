import numpy as np
from time import time

import moderngl
import moderngl_window as mglw


class Window(mglw.WindowConfig):
    gl_version = (3, 3)
    title = "ModernGL Window"
    window_size = (1280, 720)
    aspect_ratio = 16 / 9
    resizable = True
    samples = 4  # anti-aliasing
    cursor = True
    point_size = 100


# ======================================================
# -----------------visualization------------------------
class ModernGLDemo(Window):
    gl_version = (3, 3)
    title = "demo"

    def __init__(self, **kwargs):
        super().__init__(**kwargs)

        self.prog = self.ctx.program(
            vertex_shader='''
                #version 330

                in vec2 vert;
                in vec3 vert_color;
                
                out vec4 frag_color;

                uniform float scale_global;
                uniform vec2 scale_window;
                // uniform float rotation;

                void main() {
                    frag_color = vec4(vert_color, 0.8);
                    // float r = rotation * (0.5 + gl_InstanceID * 0.05);
                    // mat2 rot = mat2(cos(r), sin(r), -sin(r), cos(r));
                    gl_Position = vec4(vert * scale_window / scale_global, 0.0, 1.0);
                }
            ''',
            fragment_shader='''
                #version 330
                in vec4 frag_color;
                out vec4 color;
                void main() {
                    color = vec4(frag_color);
                }
            ''',
        )

        self.scale_window = self.prog['scale_window']   # resize window
        self.scale_global = self.prog["scale_global"]   # zoom in/out
        # self.color = self.prog['vert_color']
        # self.rotation = self.prog['rotation']
        self.scale_window.value = (0.5, self.aspect_ratio * 0.5)
        self.scale_global.value = 1
        self.make_grid()

    def make_grid(self, n=60):
        '''
        make a grid with n*2+1 lines
        n: half of the number of lines  
        '''
        self.grid = np.array([], dtype=np.float32)
        for i in range(n * 2 + 1):
            self.grid = np.append(self.grid, [i - n, -n])
            self.grid = np.append(self.grid, [0.5, 0.5, 0.5])
            self.grid = np.append(self.grid, [i - n, n])
            self.grid = np.append(self.grid, [0.5, 0.5, 0.5])
            self.grid = np.append(self.grid, [-n, i - n])
            self.grid = np.append(self.grid, [0.5, 0.5, 0.5])
            self.grid = np.append(self.grid, [n, i - n])
            self.grid = np.append(self.grid, [0.5, 0.5, 0.5])
        self.grid = self.grid.astype(np.float32)

    def draw_grid(self):
        self.vbo = self.ctx.buffer(self.grid.tobytes())
        self.vao = self.ctx.simple_vertex_array(
            self.prog, self.vbo, 'vert', 'vert_color')
        self.vao.render(moderngl.LINES)

    def draw_rectangles(self, center: np.ndarray, shape: np.ndarray):
        if center.shape[0] != shape.shape[0]:
            raise ValueError("center.shape[0] != shape.shape[0]")
        
        vert = np.array([], dtype=np.float32)
        np.random.seed(1)   # random color
        for i in range(self.n):
            color = np.random.rand(3)
            # 4 nodes of a rectangle (anti-clockwise)
            nodes = [center[i] - shape[i] / 2,
                     center[i] + np.array([shape[i, 0], -shape[i, 1]]) / 2,
                     center[i] + shape[i] / 2,
                     center[i] + np.array([-shape[i, 0], shape[i, 1]]) / 2]
            indicies = np.take(nodes, [0, 1, 3, 1, 2, 3], axis=0)
            for v in indicies:
                vert = np.append(vert, v)
                vert = np.append(vert, color)

        # start draw
        self.vbo = self.ctx.buffer(vert.astype(np.float32))
        self.vao = self.ctx.vertex_array(
            self.prog, self.vbo, 'vert', 'vert_color')
        self.vao.render(moderngl.TRIANGLES)

    def draw_connections(self, p: np.ndarray, adj: np.ndarray):
        if adj.shape[0] != p.shape[0]:
            raise ValueError("adj.shape[0] != center.shape[0]")

        n = p.shape[0]
        vert = np.array([], dtype=np.float32)
        color = np.array([.1, .0, .1])
        for i in range(n):
            for j in range(i+1, n):
                if adj[i, j] != 0:
                    vert = np.append(vert, p[i])
                    vert = np.append(vert, color)
                    vert = np.append(vert, p[j])
                    vert = np.append(vert, color)

        # print(vert.shape)
        self.vbo = self.ctx.buffer(vert.astype(np.float32))
        self.vao = self.ctx.vertex_array(
            self.prog, self.vbo, 'vert', 'vert_color')
        self.vao.render(moderngl.LINES)

    def draw_points(self, p: np.ndarray):
        n = p.shape[0]
        vert = np.array([])
        color = np.array([.0, .0, .0])  # black
        for i in range(n):
            vert = np.append(vert, p[i])
            vert = np.append(vert, color)

        self.vbo = self.ctx.buffer(vert.astype(np.float32))
        self.vao = self.ctx.vertex_array(
            self.prog, self.vbo, 'vert', 'vert_color')
        self.vao.render(moderngl.POINTS, 100)

    # ----------------------------------------------

    def render(self, time: float, frame_time: float):
        # clear screen
        self.ctx.clear(1.0, 1.0, 1.0)
        self.ctx.enable(moderngl.BLEND)

        # ==================
        # add your code here

        # draw grids
        self.draw_grid()
        # draw geometries
        # self.draw_rectangles()

        # draw connections
        # self.draw_connections()

        # self.draw_center()
        p = np.array([[0, 0], [1, 1], [2, 2], [3, 3]], dtype=np.float32)
        self.draw_points(p)

        # if time > 1:
        #     self.close()


if __name__ == "__main__":
    # np.random.seed(0)
    ModernGLDemo.run()
