#ifdef GL_ES
precision mediump float;
#endif

uniform vec2 u_resolution;

float circleShape(vec2 position, float radius) {
    return step(radius, length(position - 0.5));
}

void main() {
    vec2 position = gl_FragCoord.xy / u_resolution;
    vec3 color = vec3(0.6784, 0.2196, 0.2196);
    float circle = circleShape(position, 0.3);
    color *= circle;
    gl_FragColor = vec4(color, 1.0);
}