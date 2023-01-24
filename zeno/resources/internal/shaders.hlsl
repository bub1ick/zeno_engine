/* matrix buffer */
cbuffer matrix_buffer : register(b0)
{
  matrix world_matrix;
  matrix view_matrix;
  matrix projection_matrix;
};

/* outputs from vertex shader go here. can be interpolated to pixel shader */
struct vs_out
{
  float4 position : SV_Position; // required output of VS
  float4 color : COLOR;
};

vs_out vs_main(float4 in_position : POSITION, float4 in_color : COLOR)
{
  vs_out output = (vs_out)0; // zero the memory first

  output.position = mul(in_position, world_matrix);
  output.position = mul(output.position, view_matrix);
  output.position = mul(output.position, projection_matrix);

  output.color = in_color;

  return output;
}

float4 ps_main(vs_out input) : SV_Target {
  return input.color; // must return an RGBA colour
}