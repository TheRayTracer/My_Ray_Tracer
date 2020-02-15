my_ray_tracer.exe -input ..\scenes\glass_sphere_with_checkerboard.txt -size 512 512 -output ..\results\glass_sphere_with_checkerboard.bmp -shadows -bounces 5 -weight 0.01

my_ray_tracer.exe -input ..\scenes\glass_sphere_with_checkerboard.txt -size 512 512 -output ..\results\glass_sphere_with_checkerboard_filter_tent.bmp -shadows -bounces 5 -weight 0.01 -filter Tent 0.4

my_ray_tracer.exe -input ..\scenes\glass_sphere_with_checkerboard.txt -size 512 512 -output ..\results\glass_sphere_with_checkerboard_filter_gaussian.bmp -shadows -bounces 5 -weight 0.01 -filter Gaussian 0.4

pause