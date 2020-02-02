my_ray_tracer.exe -input reflective_teapot.txt           -size 512 512 -output reflective_teapot_no_bounce.bmp -shadows -bounces 0 -weight 0.01

my_ray_tracer.exe -input reflective_teapot.txt           -size 512 512 -output reflective_teapot.bmp           -shadows -bounces 3 -weight 0.01
my_ray_tracer.exe -input reflective_teapot_and_floor.txt -size 512 512 -output reflective_teapot_and_floor.bmp -shadows -bounces 3 -weight 0.01
my_ray_tracer.exe -input glossy_teapot.txt               -size 512 512 -output glossy_teapot.bmp               -shadows -bounces 3 -weight 0.01 -samples 128
my_ray_tracer.exe -input glossy_teapot_and_floor.txt     -size 512 512 -output glossy_teapot_and_floor.bmp     -shadows -bounces 1 -weight 0.01 -samples 128

pause