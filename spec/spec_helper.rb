require 'jiggle'
include Jiggle

Spec::Matchers.create :contain do |point|
  match do |thing|
    thing.contains? point
  end
end

def box(attr={})
  shape = [jgv(-10, -10),
           jgv(-10,  10),
           jgv( 10,  10),
           jgv( 10, -10)]
  Body.new shape, attr
end
