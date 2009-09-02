require 'spec_helper'

describe Collision do
  before :each do
    @space = Space.new
    @space.gravity = jgv(0, 9.8)

    @box = box
    @space.add @box
    
    @particle = Particle.new jgv(0, 0)
    @space.add_particle @particle
  end
  
  it "notices when there's a point inside an area" do
    @space.step(0.1)
    
    @space.collisions.should_not be_empty
  end
end
