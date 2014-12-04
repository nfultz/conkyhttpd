# 


##Instructions

Compile the C library

    gcc -shared -fPIC -llua conkyhttpd.c -o conkyhttpd.so

Then run conky:

    conky -c webconky &

And checkout the sample page:

    firefox startpage.html


