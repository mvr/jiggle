require 'jiggle/body'

module Jiggle
  
  class RotatingBody < Body
    Infinity = 1.0/0 # AGAIN AGAIN!
    
    def initialize(points, attr={})
      super(points, attr)

      anchor_strength = attr[:anchor_strength] || 200
      anchor_damping  = attr[:anchor_damping]  || 40

      anchor = Particle.new @areas[0].center_of_mass, :mass => Infinity
      anchor.collidable = false
      
      springs = @particles.map do |particle|
        Spring.new particle, anchor, :strength => anchor_strength,
                                     :damping => anchor_damping
      end

      @particles << anchor
      @springs   += springs
      
    end
  end
  
end
