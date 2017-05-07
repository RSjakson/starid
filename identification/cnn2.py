### *cnn2* cnn using libstarid
###
import random
import numpy as np
import tensorflow as tf
import libstarid.libstarid as ls
libstarid = ls.libstarid()
stars = 100
batch = 100
batches = 200

def inputs(batch, stars):
    images = np.zeros((batch, 28, 28, 1), dtype=np.float32)
    labels = np.zeros((batch), dtype=np.int32)
    for cnt in range(batch):
        starndx = random.randint(0, stars-1)
        images[cnt, :, :, 0] = libstarid.image(starndx)
        labels[cnt] = starndx
    return images, labels

data = tf.placeholder(tf.float32, [batch, 28, 28, 1])
target = tf.placeholder(tf.int32, [batch])
w1 = tf.Variable(tf.truncated_normal([5, 5, 1, 32], stddev=0.1), dtype=tf.float32)
b1 = tf.Variable(tf.constant(0.1, shape=[32]), dtype=tf.float32)
w2 = tf.Variable(tf.truncated_normal([5, 5, 32, 64], stddev=0.1), dtype=tf.float32)
b2 = tf.Variable(tf.constant(0.1, shape=[64]), dtype=tf.float32)
w3 = tf.Variable(tf.truncated_normal([7 * 7 * 64, 1024], stddev=0.1), dtype=tf.float32)
b3 = tf.Variable(tf.constant(0.1, shape=[1024]), dtype=tf.float32)
w4 = tf.Variable(tf.truncated_normal([1024, stars], stddev=0.1), dtype=tf.float32)
b4 = tf.Variable(tf.constant(0.1, shape=[stars]), dtype=tf.float32)
conv1 = tf.nn.conv2d(data, w1, strides=[1, 1, 1, 1], padding='SAME') + b1
pool1 = tf.nn.max_pool(tf.nn.relu(conv1), ksize=[1, 2, 2, 1], strides=[1, 2, 2, 1], padding='SAME')
conv2 = tf.nn.conv2d(pool1, w2, strides=[1, 1, 1, 1], padding='SAME') + b2
pool2 = tf.nn.max_pool(tf.nn.relu(conv2), ksize=[1, 2, 2, 1], strides=[1, 2, 2, 1], padding='SAME')
full3 = tf.matmul(tf.reshape(pool2, [-1, 7 * 7 * 64]), w3) + b3
drop3 = tf.nn.dropout(tf.nn.relu(full3), 1.0)
logits = tf.matmul(drop3, w4) + b4

cross_entropy = tf.reduce_mean(tf.nn.sparse_softmax_cross_entropy_with_logits(logits, target))
minimize = tf.train.AdamOptimizer().minimize(cross_entropy)
prediction = tf.cast(tf.arg_max((logits), 1), tf.int32)
accuracy = tf.reduce_mean(tf.cast(tf.equal(prediction, target), tf.float32))
saver = tf.train.Saver()
init = tf.global_variables_initializer()
sess = tf.Session()
sess.run(init)
for batchndx in range(batches):
    images, labels = inputs(batch, stars)
    sess.run(minimize, {data: images, target: labels})
    if batchndx % 10 == 0:
        images, labels = inputs(batch, stars)
        print('%5d %5.2f %5.2f' % (batchndx, sess.run(cross_entropy, {data: images, target: labels}), sess.run(accuracy, {data: images, target: labels})))
saver.save(sess, 'data_cnn2/model', global_step=batchndx)
sess.close()
