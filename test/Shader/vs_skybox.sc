$input a_position
$output v_pos

#include "../common/common.sh"

void main()
{
	v_pos = a_position;
    #if BGFX_SHADER_LANGUAGE_HLSL 
	mat4 matView = mat4(mat3(u_view));
	#else
	mat4 matView = mat4(mat3(u_view));
	#endif
	mat4 viewProj = mul(u_proj,matView);
	vec4 clipPos = mul(viewProj, vec4(a_position, 1.0) );
	gl_Position = clipPos.xyww;
}
