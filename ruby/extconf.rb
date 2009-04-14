require 'mkmf'

dir_config('jiggle_ext')

$CFLAGS << ' -I../src -O3 -std=gnu99 -Wall -Werror'
$LDFLAGS << ' -L.. -ljiggle'

create_makefile('jiggle_ext')
