### *starid*
###
### demonstration, testing, and evaluation
###
import re
import subprocess

import matplotlib.pyplot as plt
import numpy as np
import tensorflow as tf

### *id1* uses a convolutional network with training and evaluation input files based on the classic mnist format.
###
def lb(starndx):
    FLAGS = tf.app.flags.FLAGS
    tf.app.flags.DEFINE_string('checkpoint_dir', '/home/noah/starid/id1/adata', '')
    imgndx = starndx + 10 * np.random.randint(0, 1000)
    tf.reset_default_graph()
    images = lb.acontainer_make.read_images('/home/noah/starid/stars/data/eval_examples')
    image = images[imgndx, :, :, 0]
    image = tf.cast(image, tf.float32) * (1. / 255) - 0.5
    image = tf.reshape(image, [28, 28, 1])
    softmax = lb.amodel.inference(image)
    saver = tf.train.Saver()
    ckpt = tf.train.get_checkpoint_state(FLAGS.checkpoint_dir)
    sess = tf.Session()
    saver.restore(sess, ckpt.model_checkpoint_path)
    softmaxval = sess.run(softmax)
    result = np.argmax(softmaxval)
    return result

### *id* uses triangular structure in the star image.
###
def rb(starndx):
    output = subprocess.check_output(['/home/noah/starid/id/id', '--starndx', str(starndx)])
    result = int(re.search(r'identification = (\d+)', output.decode('utf-8')).group(1))
    return result

### *libstarsop* generates and plots a star image numpy array.
###
def imgop(starndx):
    libstarsop = tf.load_op_library('libstarsop.so')
    with tf.Session(''):
        image = libstarsop.stars_op(np.zeros(shape=(28, 28))).eval()
    plt.matshow(-1 * image, cmap='Greys', interpolation='nearest')
    plt.show()
    return image

### *main*
###
starndx = 1
image = imgop(starndx)
reslb = lb(starndx)
resrb = rb(starndx)


