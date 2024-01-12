package main

import (
	"fmt"
	"github.com/go-gl/gl/v4.1-core/gl"
	"github.com/go-gl/glfw/v3.2/glfw"
	//"gonum.org/v1/gonum/mat"
	"log"
	"runtime"
	"strings"
	"time"
)

// https://kylewbanks.com/blog/tutorial-opengl-with-golang-part-1-hello-opengl
func main() {
	// initialize
	runtime.LockOSThread()
	window := initGLFW()
	defer glfw.Terminate()

	program := initOpenGL()

	// vertices
	var (
		p = []float32{
			0, 0.5, 0, // top
			-0.5, -0.5, 0, // left
			0.5, -0.5, 0, // right
		}
	)

	vao := makeVAO(p)
	//gl.ClearColor(1.0, 1.0, 1.0, 1.0)

	st := time.Now().Nanosecond() // fps
	for !window.ShouldClose() {

		draw(window, program, vao) // main loop

		// fps
		et := time.Now().Nanosecond()
		var fps float32 = 1e9 / float32(et-st)
		fmt.Printf("FPS: %.2f \n", fps)
		st = et
	}
}

/* initialize */
func initGLFW() *glfw.Window {
	if err := glfw.Init(); err != nil {
		log.Fatal(err)
	}

	// Window hints
	glfw.WindowHint(glfw.Resizable, glfw.True)
	glfw.WindowHint(glfw.Samples, 4) // 4x anti-aliasing
	glfw.WindowHint(glfw.ContextVersionMajor, 4)
	glfw.WindowHint(glfw.ContextVersionMinor, 1)
	glfw.WindowHint(glfw.OpenGLProfile, glfw.OpenGLCoreProfile)
	glfw.WindowHint(glfw.OpenGLForwardCompatible, glfw.True)

	window, err := glfw.CreateWindow(640, 480, "demo", nil, nil)
	if err != nil {
		log.Fatal(err)
	}

	window.MakeContextCurrent()
	return window
}

func initOpenGL() uint32 {
	if err := gl.Init(); err != nil {
		log.Fatal(err)
	}

	version := gl.GoStr(gl.GetString(gl.VERSION))
	fmt.Println("OpenGL version", version)

	vtxShader, err := compileShader(vertexShaderSource, gl.VERTEX_SHADER)
	if err != nil {
		panic(err)
	}
	fragShader, err := compileShader(fragmentShaderSource, gl.FRAGMENT_SHADER)
	if err != nil {
		panic(err)
	}

	prog := gl.CreateProgram()
	gl.AttachShader(prog, vtxShader)
	gl.AttachShader(prog, fragShader)

	gl.LinkProgram(prog)
	return prog
}

func draw(window *glfw.Window, program uint32, vao uint32) {
	gl.Clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT)
	gl.UseProgram(program)

	gl.BindVertexArray(vao)
	gl.DrawArrays(gl.TRIANGLES, 0, 5) //int32(len(p)/3
	gl.DrawArrays(gl.LINES, 0, 6)

	glfw.PollEvents()
	window.SwapBuffers()
}

func makeVAO(verts []float32) uint32 {
	var vbo uint32
	gl.GenBuffers(1, &vbo)
	gl.BindBuffer(gl.ARRAY_BUFFER, vbo)
	gl.BufferData(gl.ARRAY_BUFFER, 4*len(verts), gl.Ptr(verts), gl.STATIC_DRAW)

	var vao uint32
	gl.GenVertexArrays(1, &vao)
	gl.BindVertexArray(vao)
	gl.EnableVertexAttribArray(0)
	gl.BindBuffer(gl.ARRAY_BUFFER, vbo)
	gl.VertexAttribPointer(0, 3, gl.FLOAT, false, 0, nil)
	return vao
}

func compileShader(source string, shaderType uint32) (uint32, error) {
	shader := gl.CreateShader(shaderType)
	scr, free := gl.Strs(source)
	gl.ShaderSource(shader, 1, scr, nil)
	free()
	gl.CompileShader(shader)
	var status int32
	gl.GetShaderiv(shader, gl.COMPILE_STATUS, &status)
	if status == gl.FALSE {
		var logLength int32
		gl.GetShaderiv(shader, gl.INFO_LOG_LENGTH, &logLength)

		log := strings.Repeat("\x00", int(logLength+1))
		gl.GetShaderInfoLog(shader, logLength, nil, gl.Str(log))

		return 0, fmt.Errorf("failed to compile %v: %v", source, log)
	}

	return shader, nil
}

var vertexShaderSource = `
    #version 410
    in vec3 vp;
    void main() {
        gl_Position = vec4(vp, 1.0);
    }
` + "\x00"

var fragmentShaderSource = `
    #version 410
	//in vec3 vp;
    out vec4 frag_colour;
    void main() {
        frag_colour = vec4(1, 1, 1, 1);
    }
` + "\x00"
