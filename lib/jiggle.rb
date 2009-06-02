$: << File.dirname(__FILE__)

require 'jiggle_ext'

%w{
  area array body collision group particle quadblob rope spring static_body vector2 world
}.each do |file|
  require 'jiggle/' + file
end



