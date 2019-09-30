$input a_position
$output v_pos

#include "../common/common.sh"

void main()
{
	v_pos = a_position;
	vec4 clipPos = mul(u_modelViewProj, vec4(a_position, 1.0) );
	gl_Position = clipPos.xyww;
}
