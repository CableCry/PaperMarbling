#version 330
in vec2 fragTexCoord;
in vec4 fragColor;

uniform sampler2D texture0;
uniform vec4      colTint;
uniform float     u_time; // example uniform

out vec4 finalColor;

void main(){
  vec4 tex = texture(texture0, fragTexCoord);
  float pulse = 0.5 + 0.5 * sin(u_time);
  finalColor = vec4(tex.rgb * pulse, tex.a) * fragColor * colTint;
}
