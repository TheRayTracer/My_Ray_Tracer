my_ray_tracer.exe -input ..\scenes\shadow_test.txt                       -size 512 512 -output ..\results\shadow_test.bmp     -shadows

my_ray_tracer.exe -input ..\scenes\shadows_colored.txt                   -size 512 512 -output ..\results\shadows_colored.bmp -shadows

my_ray_tracer.exe -input ..\scenes\point_light_circle.txt                -size 512 512 -output ..\results\shadows_hard_1.bmp  -shadows 
my_ray_tracer.exe -input ..\scenes\point_light_circle_d_attenuation.txt  -size 512 512 -output ..\results\shadows_hard_2.bmp  -shadows 
my_ray_tracer.exe -input ..\scenes\point_light_circle_d2_attenuation.txt -size 512 512 -output ..\results\shadows_hard_3.bmp  -shadows 
my_ray_tracer.exe -input ..\scenes\soft_light_circle.txt                 -size 512 512 -output ..\results\shadows_soft_16.bmp -shadows -samples 16
my_ray_tracer.exe -input ..\scenes\soft_light_circle.txt                 -size 512 512 -output ..\results\shadows_soft_64.bmp -shadows -samples 64

pause