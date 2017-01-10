# speech_recognition_wrapper
Ensure that "speech_recognition_wrapper.cpp" is referencing correctly numbered .lm and .dic files in "/model/en-us" (listed under global variables). You can read the .lm and .dic files to see which words are recognised.

You can create your own language model and dictionary at <http://www.speech.cs.cmu.edu/tools/lmtool-new.html>. Copy all the resulting files to "/model/en-us" and change the files referenced in "speech_recognition_wrapper.cpp". DO NOT RENAME THOSE FILES.

To increase accuracy for your particular voice/capture environment, you can also adapt your acoustic model using this tutorial: https://youtu.be/IAHH6-t9jK0

You can adapt the acoustic model using a custom language model and dictionary as well; just replace any occurences of the default language model/dictionary to your numbered custom ones (again, DO NOT RENAME THEM, COPY THE WHOLE CUSTOM LANGUAGE MODEL/DICTIONARY TOGETHER). The transcripts in your .transcript must be in all caps, because Pocketsphinx outputs words in all caps when you use a custom language model/dictionary.
