# ThatsIt

Provide Kotlin's `it` keyword in Ruby.

It's experimental ;) Don't use in production.

## Installation

Add this line to your application's Gemfile:

```ruby
gem 'thats_it'
```

And then execute:

    $ bundle

Or install it yourself as:

    $ gem install thats_it

## Usage

You can use `it` method in a block to get a single parameter of the block like Kotlin's `it` keyword[^1].
[^1]: https://kotlinlang.org/docs/reference/lambdas.html#it-implicit-name-of-a-single-parameter


    2.yield_self { it * it }
    # => 4

    def greeting
      yield "Hello, World!"
    end
    greeting { puts it }
    # Hello, World!

## Development

After checking out the repo, run `bin/setup` to install dependencies. You can also run `bin/console` for an interactive prompt that will allow you to experiment.

To install this gem onto your local machine, run `bundle exec rake install`. To release a new version, update the version number in `version.rb`, and then run `bundle exec rake release`, which will create a git tag for the version, push git commits and tags, and push the `.gem` file to [rubygems.org](https://rubygems.org).

## Contributing

Bug reports and pull requests are welcome on GitHub at https://github.com/hanachin/thats_it. This project is intended to be a safe, welcoming space for collaboration, and contributors are expected to adhere to the [Contributor Covenant](http://contributor-covenant.org) code of conduct.

## License

The gem is available as open source under the terms of the [MIT License](https://opensource.org/licenses/MIT).

## Code of Conduct

Everyone interacting in the ThatsIt project’s codebases, issue trackers, chat rooms and mailing lists is expected to follow the [code of conduct](https://github.com/hanachin/thats_it/blob/master/CODE_OF_CONDUCT.md).
