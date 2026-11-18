.equ WIDTH, 192
.equ HEIGHT, 144

#a0 = x
#a1= y 
#a2 = color


set_pixel:
    blt a0, zero, set_pixel_done #x>=0
    
    li t0, WIDTH
    bge a0, t0, set_pixel_done #x < width

    blt a1, zero, set_pixel_done # y>=0
   
    li t0, HEIGHT
    bge a1, t0, set_pixel_done # y < height

    #offset = y * width + x
    li, t0, WIDWTH
    mul t1, a1, t0
    add t1, t1, a0

    #address = FB + offset
    li t0, FB
    add t0, t0, t1

    sb a2, 0(t0) #store the lowest 8 bits of color
    
set_pixel_done: 
   ret
    


load_template:





#restore the registers and return
load_template_done:





