#!/usr/bin/ruby

require 'lib/jiggle'
include Jiggle

w = Space.new :ticks_per_second => 200
w.gravity = jgv(0, 9.8)

box_shape = [jgv(0, 0),
             jgv(0, 1),
             jgv(1, 1),
             jgv(1, 0),
            ]

b = StaticBody.new box_shape.map {|p| p * 10}, :offset => jgv(-1, 1)
w.add b

r = Rope.new jgv(-1, -1.5), jgv(1, -1.5), :number_of_points => 50,
                                          :mass => 10,
                                          :strength => 1500
w.add r

f = FixedBody.new box_shape, :offset => jgv(3, -1)
w.add f

f = RotatingBody.new box_shape, :offset => jgv(5, -1)
w.add f

loop do
  w.update
end
