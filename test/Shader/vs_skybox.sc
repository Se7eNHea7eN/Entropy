$input a_position
$output v_pos

#include "../common/common.sh"

void main()
{
	v_pos = a_position;


	mat4 rotView = mat4(mat3(view));
	vec4 clipPos = projection * rotView * vec4(v_pos, 1.0);

	gl_Position = clipPos.xyww;
}
