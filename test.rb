#!/usr/bin/ruby

require 'lib/jiggle'
include Jiggle
require 'rubygems'
require 'rubygame'
include Rubygame

class Vector2
  def to_world
    (self - jgv(200, 200)) / 100
  end
  def to_screen
    (self * 100) + jgv(200, 200)
  end
end

Rubygame.init
screen = Screen.set_mode [400, 400]
queue = EventQueue.new()

w = World.new :ticks_per_second => 200
w.gravity = jgv(0, 9.8)


box_shape = [jgv(0, 0),
             jgv(0, 0.5),
             jgv(0.5, 0.5),
             jgv(0.5, 0),
            ]

 b = StaticBody.new box_shape.map {|p| p * 5}, :offset => jgv(-1, 1)
# b.elasticity = 10
 w.add_body b

# r = Rope.new jgv(-1, -1.5), jgv(1, -1.5), :number_of_points => 50,
#                                           :mass => 10,
#                                           :strength => 1000
# w.add_rope r

buffer = Surface.new(screen.size)

loop do
  queue.each do |event|
    case(event)
    when MouseDownEvent
      position = jgv(*event.pos).to_world
      b = Body.new box_shape, :offset => position
      w.add_body b
    end
  end
  
  buffer.fill :black
  
  w.particles.each do |particle|
    position = particle.position.to_screen
    buffer.draw_circle_s position, 2, :white
  end

  w.springs.each do |spring|
    p_a = spring.a.position.to_screen
    p_b = spring.b.position.to_screen
    buffer.draw_line p_a, p_b, :blue
  end

  w.areas.each do |area|
    area.particles.adjacent.each do |a, b|
      p_a = a.position.to_screen
      p_b = b.position.to_screen
      buffer.draw_line p_a, p_b, :red
    end
  end
  
  buffer.blit(screen, [0,0])
  w.update
  screen.update
end

Rubygame.quit
