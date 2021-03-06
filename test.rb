#!/usr/bin/ruby

require 'lib/jiggle'
include Jiggle
require 'rubygems'
require 'rubygame'
include Rubygame

class Vector2
  def to_space
    (self - jgv(200, 200)) / 50
  end
  def to_screen
    (self * 50) + jgv(200, 200)
  end
end

Rubygame.init
screen = Screen.new [600, 600]
queue = EventQueue.new()

w = Space.new :ticks_per_second => 200
w.gravity = jgv(0, 9.8)


box_shape = [jgv(0, 0),
             jgv(0, 1),
             jgv(1, 1),
             jgv(1, 0),
            ]

b = Body.new box_shape.map {|p| p * 10}, :offset => jgv(-1, 1), :static => true
w.add b

r = Rope.new jgv(-1, -1.5), jgv(1, -1.5), :number_of_points => 50,
                                          :mass => 10,
                                          :strength => 1500,
                                          :start_static => true,
                                          :end_static => true
w.add r

f = FixedBody.new box_shape, :offset => jgv(3, -1)
w.add f

f = RotatingBody.new box_shape, :offset => jgv(5, -1)
w.add f

buffer = Surface.new(screen.size)

last_time = Time.now

loop do
  queue.each do |event|
    case(event)
    when MouseDownEvent
      position = jgv(*event.pos).to_space
      b = QuadBlob.new box_shape, :offset => position
      w.add b
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
  w.update(Time.now-last_time)
  last_time = Time.now
  screen.update
end

Rubygame.quit
