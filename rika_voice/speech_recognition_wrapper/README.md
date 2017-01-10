# speech_recognition_wrapper
In your pocketsphinx installer folder, there is a folder named "/model". Copy it here. Then, copy the contents of "/custom_dictionary" to "/model/en-us". Finally, ensure that "speech_recognition_wrapper.cpp" is referencing correctly numbered files (listed under global variables). You can read the .lm and .dic files to see which words are recognised.

You can create your own language model and dictionary at <http://www.speech.cs.cmu.edu/tools/lmtool-new.html>. Copy all the resulting files to "/model/en-us" and change the files referenced in "speech_recognition_wrapper.cpp" again. DO NOT RENAME THOSE FILES.
