$input v_pos, v_normal, v_texcoord0
#include "light.sc"

#include "../common/common.sh"

void main()
{
	gl_FragColor = vec4(v_normal.xyz,1.0);
	//gl_FragColor = vec4(1.0,1.0,1.0,1.0);
}
