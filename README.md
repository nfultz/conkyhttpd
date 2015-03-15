# Conky HTTPd

This module redirects conky output to an http connection. You might want 
to do this to create a custom lightweight monitoring service, or maybe
you want to render conky inside Firefox.


##Instructions

Compile the C library

    gcc -shared -fPIC -llua conkyhttpd.c -o conkyhttpd.so

Then run conky:

    conky -c conkyhttpd.conkyrc &

And view the sample page:

    firefox startpage.html


