### *cnn2* cnn using libstarid
###
import datetime
import time
import random
import numpy as np
import tensorflow as tf
import libstarid.libstarid as ls
libstarid = ls.libstarid()
stars = 1000
batch = 1000
batches = 1
dropout = 0.5
beta = 0.01
loginterval = 1 # batches
outdir = '/home/noah/run' + time.strftime('%m%d%H%M%S')

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
keep = tf.placeholder(tf.float32)
w1 = tf.Variable(tf.truncated_normal([5, 5, 1, 32], stddev=0.1), dtype=tf.float32)
b1 = tf.Variable(tf.constant(0.1, shape=[32]), dtype=tf.float32)
r1 = tf.nn.l2_loss(w1) * beta
w2 = tf.Variable(tf.truncated_normal([5, 5, 32, 64], stddev=0.1), dtype=tf.float32)
b2 = tf.Variable(tf.constant(0.1, shape=[64]), dtype=tf.float32)
r2 = tf.nn.l2_loss(w2) * beta
w3 = tf.Variable(tf.truncated_normal([7 * 7 * 64, 1024], stddev=0.1), dtype=tf.float32)
b3 = tf.Variable(tf.constant(0.1, shape=[1024]), dtype=tf.float32)
r3 = tf.nn.l2_loss(w3) * beta
w4 = tf.Variable(tf.truncated_normal([1024, stars], stddev=0.1), dtype=tf.float32)
b4 = tf.Variable(tf.constant(0.1, shape=[stars]), dtype=tf.float32)
r4 = tf.nn.l2_loss(w4) * beta
conv1 = tf.nn.conv2d(data, w1, strides=[1, 1, 1, 1], padding='SAME') + b1
pool1 = tf.nn.max_pool(tf.nn.relu(conv1), ksize=[1, 2, 2, 1], strides=[1, 2, 2, 1], padding='SAME')
conv2 = tf.nn.conv2d(pool1, w2, strides=[1, 1, 1, 1], padding='SAME') + b2
pool2 = tf.nn.max_pool(tf.nn.relu(conv2), ksize=[1, 2, 2, 1], strides=[1, 2, 2, 1], padding='SAME')
full3 = tf.matmul(tf.reshape(pool2, [-1, 7 * 7 * 64]), w3) + b3
drop3 = tf.nn.dropout(tf.nn.relu(full3), keep)
logits = tf.matmul(drop3, w4) + b4

loss = tf.reduce_mean(tf.nn.sparse_softmax_cross_entropy_with_logits(logits=logits, labels=target))
loss = tf.reduce_mean(loss + r1 + r2 + r3 + r4)
train = tf.train.AdamOptimizer().minimize(loss)
prediction = tf.cast(tf.arg_max((logits), 1), tf.int32)
accuracy = tf.reduce_mean(tf.cast(tf.equal(prediction, target), tf.float32))

tf.summary.histogram('w4', w4)
tf.summary.histogram('b4', b4)
tf.summary.histogram('r4', r4)
tf.summary.histogram('logits', logits)
tf.summary.scalar('loss', loss)
tf.summary.scalar('accuracy', accuracy)
stats = tf.summary.merge_all()
saver = tf.train.Saver()
writer = tf.summary.FileWriter(outdir)

init = tf.global_variables_initializer()
sess = tf.Session()
sess.run(init)
writer.add_graph(sess.graph)
t0 = time.time()
for batchndx in range(batches):
    trainin, trainlab = inputs(batch, stars)
    sess.run(train, {data: trainin, target: trainlab, keep: dropout})
    if batchndx % loginterval == 0:
        testin, testlab = inputs(batch, stars)
        testcost, testacc, teststats = sess.run([loss, accuracy, stats], {data: testin, target: testlab, keep: 1.0})
        writer.add_summary(teststats, batchndx)
        print('%s, %.3f, %d, %.4f, %.4f' % (time.strftime('%H%M%S'), time.time()-t0, batchndx, testcost, testacc))
saver = tf.train.Saver()
saver.save(sess, outdir+'/model', global_step=batchndx)
sess.close()
