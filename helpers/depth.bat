my_ray_tracer.exe -input ..\scenes\focal_red_spheres_near.txt   -size 512 512 -output ..\results\focal_red_spheres_near_16.bmp    -shadows -bounces 5 -weight 0.01 -samples 16
my_ray_tracer.exe -input ..\scenes\focal_red_spheres_near.txt   -size 512 512 -output ..\results\focal_red_spheres_near_64.bmp    -shadows -bounces 5 -weight 0.01 -samples 64

my_ray_tracer.exe -input ..\scenes\focal_red_spheres_near.txt   -size 512 512 -output ..\results\focal_red_spheres_near_128.bmp   -shadows -bounces 5 -weight 0.01 -samples 128
my_ray_tracer.exe -input ..\scenes\focal_red_spheres_middle.txt -size 512 512 -output ..\results\focal_red_spheres_middle_128.bmp -shadows -bounces 5 -weight 0.01 -samples 128
my_ray_tracer.exe -input ..\scenes\focal_red_spheres_far.txt    -size 512 512 -output ..\results\focal_red_spheres_far_128.bmp    -shadows -bounces 5 -weight 0.01 -samples 128

pause