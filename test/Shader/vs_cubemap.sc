$input a_position
$output v_pos

#include "../common/common.sh"


void main()
{
    v_pos = a_position;  
    gl_Position = mul(mul(u_proj,u_view), vec4(a_position, 1.0) );
}