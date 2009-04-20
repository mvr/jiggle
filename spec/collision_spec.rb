require 'spec_helper'

describe Collision do
  before :each do
    @world = World.new
    @world.gravity = jgv(0, 9.8)

    @box = box
    @world.add_body @box
    
    @particle = Particle.new jgv(0, 0)
    @world.add_particle @particle
  end
  
  it "notices when there's a point inside an area" do
    @world.step(0.1)
    
    @world.collisions.should_not be_empty
  end
end
