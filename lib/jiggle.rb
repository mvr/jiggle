$: << File.dirname(__FILE__)

require 'jiggle_ext'

%w{
  area array body collision fixed_body group particle quadblob rope rotating_body spring vector2 space
}.each do |file|
  require 'jiggle/' + file
end



