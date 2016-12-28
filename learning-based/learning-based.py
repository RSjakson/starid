from optparse import OptionParser
import mnist
import tensorflow as tf
import graph_nodes as gn

def main():
  usage = "usage: %prog [options] arg"
  parser = OptionParser(usage)
  parser.add_option('-s', '--starndx', dest='starndx', help='')
  (options, args) = parser.parse_args()

  FLAGS = tf.app.flags.FLAGS
  tf.app.flags.DEFINE_string('checkpoint_dir', '/home/noah/dev/train', 'model dir')
  images = mnist.read_images('/home/noah/dev/starid/data/images_b1.mnist')
  labels = mnist.read_labels('/home/noah/dev/starid/data/images_b2.mnist')
  imgndx = int(int(options.starndx) / 800) - 1
  label = labels[imgndx] # label value in file is (starndx/800)-1, for starndxs 800, 1600, ... 8000
  image = images[imgndx,:,:,0]
  image = tf.cast(image,  if options.starndx:
    print("starndx %s" % options.starndx)
 tf.float32) * (1. / 255) - 0.5
  image = tf.reshape(image, [28, 28, 1])
  softmax = gn.inference(image)
  saver = tf.train.Saver()
  ckpt = tf.train.get_checkpoint_state(FLAGS.checkpoint_dir)
  with tf.Session() as sess:
    saver.restore(sess, ckpt.model_checkpoint_path)
    softmaxval = sess.run(softmax)
    print(softmaxval)

if __name__ == "__main__":
  main()