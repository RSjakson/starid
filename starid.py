### *starid*
###
### test and demo
###

import libstarid.libstarid as ls
import id.id as id
libstarid = ls.libstarid()
libstarid.load_pairs()

image = libstarid.image(starndx=1)
print(libstarid.id(image))
print(id.tf1(image))


