# Vertocoin Official Development Repo

[![Vertocoin Donate](https://badgen.net/badge/vertocoin/Donate/green)](#)
[![Continuous Integration](https://github.com/vertocoin/vertocoin/actions/workflows/build.yml/badge.svg?branch=master)](https://github.com/vertocoin/vertocoin/actions/workflows/build.yml)

### What is Vertocoin?

[Vertocoin](https://vertocoin.net) (abbreviated VTO) is a high-speed private cryptocurrency designed for the future. Built on a proven blockchain foundation, Vertocoin introduces lightning-fast transaction processing with 1-minute block times and enhanced privacy features. With a maximum supply of 50 billion VTO, Vertocoin aims to be the ideal cryptocurrency for everyday transactions while maintaining strong security through proof-of-stake consensus.

### Key Features

- **High Speed**: 1-minute block times for rapid transaction confirmation
- **Large Supply**: 50 billion VTO maximum supply for widespread adoption
- **Private**: Enhanced privacy features for secure transactions
- **Efficient**: Proof-of-stake consensus for energy efficiency
- **Secure**: Battle-tested blockchain technology

### Vertocoin Resources

- Client and Source:
  [Client Binaries](https://github.com/vertocoin/vertocoin/releases),
  [Source Code](https://github.com/vertocoin/vertocoin)
- Documentation: [Vertocoin Docs](https://docs.vertocoin.net)
- Help:
  [Forum](https://talk.vertocoin.net),
  [Intro & Important Links](https://talk.vertocoin.net/t/what-is-vertocoin-intro-important-links/1),
  [Telegram Chat](https://t.me/vertocoin)

### Network Information

- **Main Network P2P Port**: 9333
- **Main Network RPC Port**: 9332
- **Testnet P2P Port**: 19333
- **Testnet RPC Port**: 19332

## Testing

Testing and code review is the bottleneck for development; we get more pull
requests than we can review and test. Please be patient and help out, and
remember this is a security-critical project where any mistake might cost people
lots of money.

### Automated Testing

Developers are strongly encouraged to write unit tests for new code, and to submit new unit tests for old code.

Unit tests can be compiled and run (assuming they weren't disabled in configure) with:
make check

### Manual Quality Assurance (QA) Testing

Large changes should have a test plan, and should be tested by somebody other than the developer who wrote the code.

- Developers work in their own forks, then submit pull requests when they think their feature or bug fix is ready.
- If it is a simple/trivial/non-controversial change, then one of the development team members simply pulls it.
- If it is a more complicated or potentially controversial change, then the change may be discussed in the pull request, or the requester may be asked to start a discussion in the [Peercoin Forum](https://talk.peercoin.net) for a broader community discussion.
- The patch will be accepted if there is broad consensus that it is a good thing. Developers should expect to rework and resubmit patches if they don't match the project's coding conventions (see coding.txt) or are controversial.
- From time to time a pull request will become outdated. If this occurs, and the pull is no longer automatically mergeable; a comment on the pull will be used to issue a warning of closure. Pull requests closed in this manner will have their corresponding issue labeled 'stagnant'.
- For development ideas and help see [here](https://talk.peercoin.net/c/protocol).

## Branches:

### develop (all pull requests should go here)

The develop branch is used by developers to merge their newly implemented features to.
Pull requests should always be made to this branch (except for critical fixes), and could possibly break the code.
The develop branch is therefore unstable and not guaranteed to work on any system.

### master (only updated by group members)

The master branch get's updates from tested states of the develop branch.
Therefore, the master branch should contain functional but experimental code.

### release-\* (the official releases)

The release branch is identified by it's major and minor version number e.g. `release-0.6`.
The official release tags are always made on a release branch.
Release branches will typically branch from or merge tested code from the master branch to freeze the code for release.
Only critical patches can be applied through pull requests directly on this branch, all non critical features should follow the standard path through develop -> master -> release-\*
