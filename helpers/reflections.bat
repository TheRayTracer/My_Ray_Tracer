my_ray_tracer.exe -input ..\scenes\reflective_big_sphere.txt     -size 512 512 -output ..\results\reflective_big_sphere_1.bmp       -shadows -bounces 0 -weight 0.01
my_ray_tracer.exe -input ..\scenes\reflective_big_sphere.txt     -size 512 512 -output ..\results\reflective_big_sphere_2.bmp       -shadows -bounces 1 -weight 0.01
my_ray_tracer.exe -input ..\scenes\reflective_big_sphere.txt     -size 512 512 -output ..\results\reflective_big_sphere_3.bmp       -shadows -bounces 2 -weight 0.01
my_ray_tracer.exe -input ..\scenes\reflective_big_sphere.txt     -size 512 512 -output ..\results\reflective_big_sphere_4.bmp       -shadows -bounces 3 -weight 0.01

my_ray_tracer.exe -input ..\scenes\glossy_red_and_big_spheres_1.txt -size 512 512 -output ..\results\glossy_red_and_big_spheres.bmp       -shadows -bounces 5 -weight 0.01
my_ray_tracer.exe -input ..\scenes\glossy_red_and_big_spheres_2.txt -size 512 512 -output ..\results\glossy_red_and_big_spheres_a_128.bmp -shadows -bounces 5 -weight 0.01 -samples 128
my_ray_tracer.exe -input ..\scenes\glossy_red_and_big_spheres_3.txt -size 512 512 -output ..\results\glossy_red_and_big_spheres_b_16.bmp  -shadows -bounces 5 -weight 0.01 -samples 16
my_ray_tracer.exe -input ..\scenes\glossy_red_and_big_spheres_3.txt -size 512 512 -output ..\results\glossy_red_and_big_spheres_b_64.bmp  -shadows -bounces 5 -weight 0.01 -samples 64
my_ray_tracer.exe -input ..\scenes\glossy_red_and_big_spheres_3.txt -size 512 512 -output ..\results\glossy_red_and_big_spheres_b_128.bmp -shadows -bounces 5 -weight 0.01 -samples 128

my_ray_tracer.exe -input ..\scenes\glossy_red_spheres_1.txt        -size 512 512 -output ..\results\glossy_red_spheres.bmp              -shadows -bounces 5 -weight 0.01
my_ray_tracer.exe -input ..\scenes\glossy_red_spheres_2.txt        -size 512 512 -output ..\results\glossy_red_spheres_a_128.bmp        -shadows -bounces 5 -weight 0.01 -samples 128
my_ray_tracer.exe -input ..\scenes\glossy_red_spheres_3.txt        -size 512 512 -output ..\results\glossy_red_spheres_b_128.bmp        -shadows -bounces 5 -weight 0.01 -samples 128

my_ray_tracer.exe -input ..\scenes\glossy_color_spheres_1.txt      -size 512 512 -output ..\results\glossy_color_spheres.bmp            -shadows -bounces 5 -weight 0.01
my_ray_tracer.exe -input ..\scenes\glossy_color_spheres_2.txt      -size 512 512 -output ..\results\glossy_color_spheres_a_128.bmp      -shadows -bounces 5 -weight 0.01 -samples 128
my_ray_tracer.exe -input ..\scenes\glossy_color_spheres_3.txt      -size 512 512 -output ..\results\glossy_color_spheres_b_128.bmp      -shadows -bounces 5 -weight 0.01 -samples 128

my_ray_tracer.exe -input ..\scenes\shapes_scene.txt               -size 512 512 -output ..\results\shapes_scene.bmp                   -shadows -bounces 3 -weight 0.01 -samples 32

pause