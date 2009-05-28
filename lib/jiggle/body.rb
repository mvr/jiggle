require 'jiggle/group'

module Jiggle
  
  class Body < Group
    def initialize(points, attr={})
      super()
      
      attr[:mass] ||= points.size
      mass = attr[:mass] / points.size
      
      scale  = attr[:scale]  || 1
      offset = attr[:offset] || jgv(0, 0)

      points = points.map { |point| (point * scale) + offset }
      
      points.each do |point|
        particle = Particle.new point, :mass => mass
        @particles << particle
      end

      @particles.pairs.each do |p1, p2|
        spring = Spring.new p1, p2, :strength => attr[:strength], :damping => attr[:damping]
        @springs << spring
      end

      @areas = [Area.new(@particles)]
    end
  end
end
