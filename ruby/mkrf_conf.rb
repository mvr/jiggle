require 'mkrf'
Mkrf::Generator.new('jiggle') do |g|
  g.cflags << '-I../src -O3 -std=gnu99 -Wall -Werror'
  g.objects << '-L.. -ljiggle'
  g.ldshared << '--enable-auto-import'
end
