### *starid*
###
### demonstration, testing, and evaluation
###
import subprocess
import re
import time
import numpy as np
import tensorflow as tf
import lb.amodel
import lb.acontainer_make

FLAGS = tf.app.flags.FLAGS
tf.app.flags.DEFINE_string('checkpoint_dir', '/home/noah/starid/lb/adata', '')
resultscnt = 5
results = np.zeros(shape=(resultscnt, 6), dtype=float)


def identify_central_star(imgndx):
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
    ndx = np.argmax(softmaxval)
    return ndx


for resultsndx in range(0, resultscnt):
    starndx = np.mod(resultsndx, 10)
    starsetndx = np.random.randint(0, 1000)
    imgndx = starndx + starsetndx * 10
    results[resultsndx, 0] = starndx
    results[resultsndx, 1] = imgndx

    t1 = time.time()
    starndx1 = identify_central_star(imgndx)
    if starndx1 == results[resultsndx, 0]:
        results[resultsndx, 2] = 1
    results[resultsndx, 3] = float(time.time() - t1)

    t2 = time.time()
    output = subprocess.check_output(['/home/noah/starid/rb/rb', '--imgndx', str(imgndx)])
    starndx2 = int(re.search(r'identification = (\d+)', output.decode('utf-8')).group(1))
    if starndx2 == results[resultsndx, 0]:
        results[resultsndx, 4] = 1
    results[resultsndx, 5] = float(time.time() - t2)

    print('%5.0f, %5.0f, %1.0f, %1.0f' % (
    results[resultsndx, 0], results[resultsndx, 1], results[resultsndx, 2], results[resultsndx, 4]))

print()
print('%5s  %5s  %5s' % ('n %i' % resultscnt, 'ok', 't'))
print('%5s  %5s  %5s' % ('-----', '-----', '-----'))
print('%5s  %4.3f  %4.3f' % ('lb', np.mean(results[:, 2]), np.mean(results[:, 3])))
print('%5s  %4.3f  %4.3f' % ('rb', np.mean(results[:, 4]), np.mean(results[:, 5])))