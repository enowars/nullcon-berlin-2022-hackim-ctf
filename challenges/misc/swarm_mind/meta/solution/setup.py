from PIL import Image
import random

save_x = (50,490)
save_y = (25, 60)
new_height = 137

def prime(n):
	return all([n % i != 0 for i in range(2,n)])
 
im = Image.open('swarm.png')
width, height = im.size
assert(prime(width))
assert(prime(height))

im = im.crop((0,100,width, 100 + new_height))
width, height = im.size
assert(prime(width))
assert(prime(height))

pixel = im.load()

n = ''
for x in range(width):
	pixel[x,0] = 255
	n += '1'
	for y in range(1,height-1):
		if x < save_x[0] or x > save_x[1] or y < save_y[0] or y > save_y[1]:
			pixel[x,y] = 255 * round(random.random())
		n += str(pixel[x,y] // 255)
	pixel[x,-1] = 255
	n += '1'
im.save('hidden_swarm.png')

writer = open('swarm.txt','w')
writer.write('Dear swarm, what am I supposed to do with this number?\n')
writer.write('%d' % int(n,2))
writer.close()
