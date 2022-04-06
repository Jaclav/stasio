#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <SFML/Graphics.hpp>

int main(int argc, char **argv) {
    sf::Window window(sf::VideoMode::getDesktopMode(), "stasio.exe", sf::Style::None, sf::ContextSettings(24, 8, 8, 4, 6));
    sf::Event event;

    glewInit();

    GLfloat points[] = {
        -0.5f, -0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            0.5f, 0.5f, 0.0f,
            0.5f, 0.5f, 0.0f,
            -0.5f, 0.5f, 0.0f,
            -0.5f, -0.5f, 0.0f,
        };
    GLuint position_vbo = 0;
    glGenBuffers(1, &position_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, position_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

    GLfloat tex_coords[] = {
        0.0f, 0.0f, // A
        1.0f, 0.0f, // B
        1.0f, 1.0f, // C
        1.0f, 1.0f, // D
        0.0f, 1.0f, // E
        0.0f, 0.0f, // F
    };
    GLuint texture_coords_vbo = 0;
    glGenBuffers(1, &texture_coords_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, texture_coords_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(tex_coords), tex_coords, GL_STATIC_DRAW);

    GLuint vao = 0;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, position_vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glBindBuffer(GL_ARRAY_BUFFER, texture_coords_vbo);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    sf::Shader shader;
    shader.loadFromFile("shader.vert", "shader.frag");

    sf::Texture textureT;
    if(argc == 1)
        textureT.loadFromFile("stasio.png");
    else
        textureT.loadFromFile("papaj.jpg");

	textureT.setSmooth(true);
    shader.setUniform("basic_texture", textureT);

    glm::mat4 translate_matrix(glm::translate(glm::mat4(1.0), glm::vec3(0, 0, 0)));
    glm::mat4 scale_matrix(glm::scale(translate_matrix, glm::vec3(1, -1, 1)));
    float tmp;
    glm::mat4 rotate_matrix(glm::rotate(scale_matrix, 0.0f, glm::vec3(0, 1, 0)));

    shader.setUniform("transformation", sf::Glsl::Mat4(glm::value_ptr(rotate_matrix)));

    while(window.isOpen()) {
        while(window.pollEvent(event)) {
            if(event.type == sf::Event::Closed) {
                window.close();
            }
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
                if(window.getSize().x == 500)
                    window.setSize(sf::Vector2u(sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height));
                else
                    window.setSize(sf::Vector2u(500, 500));

                glViewport(0, 0, window.getSize().x, window.getSize().y);
                sf::sleep(sf::milliseconds(200));
                break;
            }
        }

        glClearColor(0.5f, 0.3f, 0.3f, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        translate_matrix = glm::mat4(glm::translate(glm::mat4(1.0), glm::vec3(sin(tmp) / 2, cos(tmp) / 2, 0)));
        scale_matrix = glm::mat4(glm::scale(translate_matrix, glm::vec3(1, -1, 1)));

        rotate_matrix = glm::mat4(glm::rotate(scale_matrix, tmp, glm::vec3(1, 0, 0)));
        rotate_matrix = glm::mat4(glm::rotate(rotate_matrix, tmp, glm::vec3(0, 1, 0)));
        rotate_matrix = glm::mat4(glm::rotate(rotate_matrix, tmp, glm::vec3(0, 0, 1)));
        tmp += 0.02;

        shader.setUniform("transformation", sf::Glsl::Mat4(glm::value_ptr(rotate_matrix)));
        sf::Shader::bind(&shader);
        sf::sleep(sf::milliseconds(40));

        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, sizeof(points) / 3);

        window.display();
    }
}
