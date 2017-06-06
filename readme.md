<img src="docs/images/star4a.png" align="right" height="200" width="200"/><img src="docs/images/star4b.png" align="right" height="200" width="200"/>

[about](http://starid.org/about), [references](http://starid.org/references)

*stars* - image generation from the nasa skymap catalog - [guide](https://drive.google.com/file/d/0B50jA_ROMYdHRjF6VUhKTkxvU0U/view?usp=sharing), [catalog](https://drive.google.com/file/d/0B50jA_ROMYdHMTNoenMzYkpNdXc/view?usp=sharing)

*identification* - recurrent neural networks, convnets, triangular patterns and structure - [recurrent network](https://github.com/noahhsmith/starid/blob/master/identification/recurrent_minimalist.py), [experimental rnn](https://github.com/noahhsmith/starid/blob/master/identification/recurrent.py), [convnet](https://github.com/noahhsmith/starid/blob/master/identification/convolutional_minimalist.py), [experimental cnn](https://github.com/noahhsmith/starid/blob/master/identification/convolutional.py), [triangle heuristics](https://github.com/noahhsmith/starid/blob/master/identification/triangles.h)

<br>

*recurrent neural networks* - recurrent networks are about sequential processing and patterns in sequential data. they’re natural for time series and time varying systems. sequences may be an indirect perspective for star images, but we're concerned with randomly rotated star images. rotations shouldn't effect image sequences, and recurrent networks were capable of identifying thousands of stars before convolutional networks.

*convolutional networks* - the concept here is static unchanging data and patterns, as naturally found in images. this at first glance appear to be a direct solution for star identification, but our star images are randomly rotated. for a given star, you can't simply overlay one image on another and see how well they match.

*triangular patterns and structure* - find star triangles in the image, and candidate stars that meet the constraints implicit within the triangle. these constraints are used in practice as rules for rejecting candidate stars that can't be correct. for a particular image, we start with 8,876 candidate identifications and reject 8,875 of them as impossible, leaving the correct result.
