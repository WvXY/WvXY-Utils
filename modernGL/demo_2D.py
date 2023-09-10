from time import time

import moderngl
import moderngl_window as mglw
import numpy as np


class Window(mglw.WindowConfig):
    gl_version = (3, 3)
    title = "ModernGL Window"
    window_size = (1280, 720)
    aspect_ratio = 16 / 9
    resizable = True
    samples = 1  # anti-aliasing
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

        self.scale_window = self.prog['scale_window']  # resize window
        self.scale_global = self.prog["scale_global"]  # zoom in/out
        # self.color = self.prog['vert_color']
        # self.rotation = self.prog['rotation']
        self.scale_window.value = (0.5, self.aspect_ratio * 0.5)
        self.scale_global.value = 10
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
        '''
        draw rectangles
        :param center:  center of rectangles
        :param shape:   shape of rectangles (width, height)
        :return:        None
        '''
        if center.shape[0] != shape.shape[0]:
            raise ValueError("center.shape[0] != shape.shape[0]")

        vert = np.array([], dtype=np.float32)
        n = center.shape[0]
        np.random.seed(1)  # random color
        for i in range(n):
            color = np.random.rand(3)
            # 4 nodes of a rectangle (anti-clockwise)
            nodes = [center[i] - shape[i] / 2,
                     center[i] + np.array([shape[i, 0], -shape[i, 1]]) / 2,
                     center[i] + shape[i] / 2,
                     center[i] + np.array([-shape[i, 0], shape[i, 1]]) / 2]
            indices = np.take(nodes, [0, 1, 3, 1, 2, 3], axis=0)
            for v in indices:
                vert = np.append(vert, v)
                vert = np.append(vert, color)

        # start draw
        self.vbo = self.ctx.buffer(vert.astype(np.float32))
        self.vao = self.ctx.vertex_array(
            self.prog, self.vbo, 'vert', 'vert_color')
        self.vao.render(moderngl.TRIANGLES)

    def draw_circle(self, center: np.ndarray, radius: np.float32,
                    n=64, color=np.array([.0, 0., 0.])):
        '''
        draw a circle by combining triangles
        :param center:  center of circle
        :param radius:  radius of circle
        :param n:       number of points
        :param color:   color of circle
        :return:        None
        '''
        angle = np.linspace(0, 2 * np.pi, n)
        p = center + radius * np.array([np.cos(angle), np.sin(angle)]).T
        p = np.insert(p, 0, center, axis=0)
        vert = np.hstack((p, np.tile(color, (n + 1, 1))))

        self.vbo = self.ctx.buffer(vert.astype(np.float32))
        self.vao = self.ctx.vertex_array(
            self.prog, self.vbo, 'vert', 'vert_color')
        self.vao.render(moderngl.TRIANGLE_FAN)

    def draw_points(self, p: np.ndarray, size=.1):
        '''
        draw points using circles with small radius
        :param p:  points
        :param size:  radius of circles
        :return:  None
        '''
        n = p.shape[0]
        vert = np.array([])
        color = np.random.random(3) / 8  # random dark color
        for i in range(n):
            self.draw_circle(p[i], size, color=color, n=16)

    def draw_connections(self, p: np.ndarray, adj: np.ndarray):
        '''
        draw connections between points
        :param p:       points
        :param adj:     (weighted) adjacency matrix
        :return:        None
        '''
        if adj.shape[0] != p.shape[0]:
            raise ValueError("adj.shape[0] != center.shape[0]")

        n = p.shape[0]
        vert = np.array([], dtype=np.float32)
        color = np.array([.1, .0, .1])
        for i in range(n):
            for j in range(i + 1, n):
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

    # ---------------------------------------------
    # ---------------render loop-------------------
    def render(self, time: float, frame_time: float):
        # clear screen
        self.ctx.clear(1.0, 1.0, 1.0)
        self.ctx.enable(moderngl.BLEND)

        # ==================
        # add code here
        self.draw_grid()
        # self.draw_rectangles(np.array([[0, 0]]), np.array([[1, 1]]))
        self.draw_circle(np.array([0, 0]), np.array([10, 10]) * time)

        # self.draw_connections()

        # self.draw_center()
        p = np.random.randint(-10, 10, (10, 2))
        self.draw_points(p)


if __name__ == "__main__":
    # mglw.run_window_config(ModernGLDemo)
    ModernGLDemo.run()
