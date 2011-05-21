require 'jiggle/group'

module Jiggle
  
  class Speck < Group
    def initialize(position, attr={})
      super()
      
      attr[:mass] ||= 5
      
      particle = Particle.new position, :mass => attr[:mass]
      @particles << particle
    end

    def position
      @particles.first.position
    end
  end
end
