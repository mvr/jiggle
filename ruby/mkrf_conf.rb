require 'mkrf'
Mkrf::Generator.new('lib/jiggle_ext') do |g|
  g.cflags << '-I../src -O3 -std=gnu99 -Wall -Werror'
  g.objects << '-L.. -ljiggle'
  g.ldshared << '--enable-auto-import'
end
