require "bundler/gem_tasks"
require "rake/extensiontask"
require "rake/testtask"

Rake::TestTask.new(:test) do |t|
  t.libs << "test"
  t.libs << "lib"
  t.test_files = FileList['test/**/*_test.rb']
end

task :build => :compile

Rake::ExtensionTask.new("implicit_it") do |ext|
  ext.lib_dir = "lib/implicit_it"
end

task :default => [:clobber, :compile, :test]
