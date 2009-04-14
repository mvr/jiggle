require 'rake/clean'
require 'rake/loaders/makefile'
require 'spec/rake/spectask'
#import  '.depend.mf' # Import dependencies

SOURCES      = FileList.new('src/*.c')
OBJECTS      = SOURCES.ext('.o')
HEADERS      = FileList.new('src/*.h')
LIBRARY      = 'libjiggle.a'
TESTS        = FileList.new('tests/*.h')
DEMOS        = FileList.new('demos/*.c')
CCOPTIONS    = '-std=c99 -Werror -Wall -pedantic -I. -I.. -Isrc -O3'

CLEAN.include(OBJECTS)
CLEAN.include('.depend.mf')
CLEAN.include('libjiggle.a')
CLEAN.include('tests/test.o')
CLEAN.include('tests/test')

task :default => LIBRARY

file LIBRARY => OBJECTS do
  sh 'ar rcs libjiggle.a src/*.o'
end

rule '.o' => '.c' do |t|
  sh "gcc -c -o #{t.name} #{t.source} #{CCOPTIONS}"
end

# Dependencies

# file '.depend.mf' => SOURCES + HEADERS do # Make dependencies
#   sh "gcc -M #{SOURCES + HEADERS} > .depend.mf"
# end

# Tests

file 'tests/test.c' => TESTS do
  output = "#include \"test.h\"\n\n"

  TESTS.each do |name|
    output += "#include \"#{name}\"\n"
  end

  output += "int main()\n{\n"

  TESTS.each do |name|
    File.open(name).read.scan(/^test\((.*?)\)/) { |testname|
      output += "run_test(#{testname});\n"
    }
  end

  output += "}\n"

  outfile = File.open('tests/test.c', 'w')
  outfile.print output
  outfile.close
end

task :test => ['tests/test.c', LIBRARY] do
  sh "gcc -c -o tests/test.o tests/test.c #{CCOPTIONS}"
  sh "gcc -o tests/test tests/test.o -L. -ljiggle -lm"
  sh "tests/test"
end

Spec::Rake::SpecTask.new do |t|
  t.libs +=  ["lib", "spec"]
end

# Ruby

task :ruby => LIBRARY do
  system "cd ext && ruby extconf.rb && make && mv jiggle_ext.so ../lib/jiggle_ext.so"
end
