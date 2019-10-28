$input a_position
$output v_pos

#include "../common/common.sh"

void main()
{
	v_pos = a_position;
    #if BGFX_SHADER_LANGUAGE_HLSL 
	mat4 matView = u_view;
	matView[0][3] = 0.0;
	matView[1][3] = 0.0;
	matView[2][3] = 0.0;
	matView[3][3] = 0.0;
	#else
	mat4 matView = mat4(mat3(u_view));
	#endif
	mat4 viewProj = mul(u_proj,matView);
	vec4 clipPos = mul(viewProj, vec4(a_position, 1.0) );
	gl_Position = clipPos.xyww;
}
