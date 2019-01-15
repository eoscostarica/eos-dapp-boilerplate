<p align="center">
	</a>
	<a href="https://git.io/col">
		<img src="https://img.shields.io/badge/%E2%9C%93-collaborative_etiquette-brightgreen.svg" alt="Collaborative Etiquette">
	</a>
	<a href="https://discord.gg/bBpQHym">
		<img src="https://img.shields.io/discord/447118387118735380.svg?logo=discord" alt="chat on Discord">
	</a>
	<a href="https://twitter.com/intent/follow?screen_name=eoscostarica">
		<img src="https://img.shields.io/twitter/follow/eoscostarica.svg?style=social&logo=twitter" alt="follow on Twitter">
	</a>
	<a href="#">
		<img src="https://img.shields.io/dub/l/vibe-d.svg" alt="MIT">
	</a>
</p>

# EOSIO Dapp Boilerplate

This boilerplate provides a very quick way to get started with application development on the EOSIO platform.

It's an opinioned selection of technologies that will allow you to build performant and scalable EOSIO dApps. 

You can configure the project to use any testnet for development, if you are interested in running all eosio services in your machine take a look at [EOS Local](https://github.com/eoscostarica/eos-local).  

This project was inspired on MonsterEOS' **EOSIO Dream Stack** architecture.

<!-- START doctoc generated TOC please keep comment here to allow auto update -->
<!-- DON'T EDIT THIS SECTION, INSTEAD RE-RUN doctoc TO UPDATE -->
**Table of Contents**  *generated with [DocToc](https://github.com/thlorenz/doctoc)*

- [Architecture](#architecture)
- [Technical Specs](#technical-specs)
- [Getting started](#getting-started)
- [Commands](#commands)
- [Directory Structure](#directory-structure)
- [Services](#services)
  - [demux](#demux)
  - [graphql](#graphql)
    - [hasura](#hasura)
  - [postgres](#postgres)
  - [pgweb](#pgweb)
  - [reactjs web client](#reactjs-web-client)
  - [Material UI](#material-ui)
    - [components](#components)
- [Continuous Integration Process](#continuous-integration-process)
- [EOS Documentation & Learning Resources](#eos-documentation--learning-resources)
- [Frequently Asked Questions](#frequently-asked-questions)
  - [Why Containers ?](#why-containers-)
  - [Why Database Migrations ?](#why-database-migrations-)
  - [Who is using EOS Local ?](#who-is-using-eos-local-)
- [Contributing](#contributing)
- [About EOS Costa Rica](#about-eos-costa-rica)
- [License](#license)
- [Contributors](#contributors)

<!-- END doctoc generated TOC please keep comment here to allow auto update -->

## Architecture

<p align="center">
	<img src="docs/EOS-Local-Architecture.png" width="600">
</p>

## Technical Specs

- Virtualized environment.
- Microservices architecture.
- Out-of-box services: 
  - Demux service for executing side effects and data replication to postgres. 
  - GraphQL endpoint with Hasura for executing complex data queries with ease.
  - PGWeb instance for exploring the demux postgres database.
  - Postgres database for the dApp data.
  - Reactjs client with:
    - Scatter integration. 
    - Lynx integration. WIP
    - Material UI.
    - GraphQL Apollo client.
- Automated code linting and testing.
- Continuous Integration and Deployment. ( Travis and Netlify ) - CircleCI soon

*Note: at the moment we are not using a docker container for running the React client due to issues related to hot reloading the app efficiently*

**Important Disclaimer: This is a Work in Progress** 

## Getting started

Basic knowledge about Docker, Docker Compose, EOSIO and NodeJS is required.

**Global Dependencies**

- Docker https://docs.docker.com/install/.   
At least 6GB RAM (Docker -> Preferences -> Advanced -> Memory -> 6GB or above)
- Hasura CLI https://docs.hasura.io/1.0/graphql/manual/hasura-cli/install-hasura-cli.html

**Optionally**
- Install node.js v11 on your machine. We recommend using [n](https://github.com/tj/n) or [nvm](https://github.com/creationix/nvm), and [avn](https://github.com/wbyoung/avn) to manage multiple node.js versions on your computer.
- Yarn https://yarnpkg.com/lang/en/docs/install/.

## Commands

- `make start` starts all containers and the reactjs app.
- `make flush` stops and removes all cotainers and data.
- `make hasura` open the hasura console on the browser.
- `make migrate` runs hasura migration against the postgres database.
- `docker-compose build`  build all containers,
- `docker-compose up` starts all containers.
- `docker-compose up --build` rebuilds and starts all containers.
- `docker-compose exec [service_name] [bash | sh]` open bash or sh in a container.
- `docker-compose stop` stops all containers.
- `docker-compose down` stops and removes all containers.
- `docker-compose restart` restarts all services.

See the full list here https://docs.docker.com/compose/reference/

## Directory Structure

```
.
├── docs/ .............................................. documentation files and media
├── contracts/ ......................................... eosio smart contracts 
├── services/ .......................................... microservices
|   ├── demux/ ......................................... demux-js service
|   |   ├── utils/ ..................................... general utilities
|   |   ├── src/ ....................................... application biz logic 
|   |   ├── Dockerfile ................................. service image spec 
|   |   ├── pm2.config.js .............................. process specs for pm2
|   |   ├── tsconfig.json .............................. tslint config
|   |   ├── tslint.json ................................ code style rules
|   |   └── package.json ............................... service dependencies manifest
|   |
|   ├── hasura/ ........................................ graphql endpoint service
|   |   ├── migrations/ ................................ hasura postgres migrations
|   |   └── config.yaml ................................ hasura config file
|   |
|   └── frontend/ ...................................... reactjs frontend
|       ├── public/ .................................... static and public files
|       ├── src/ ....................................... reactjs views and components
|       ├── config-overrides.js ........................ configuration overrides for `cra`
|       ├── .env ....................................... environment variables
|       ├── .eslintrc .................................. code style rules
|       └── package.json ............................... service dependencies manifest
|   
├── docker-compose.yaml ................................ docker compose for local dev
├── contributing.md .................................... contributing guidelines
├── license ............................................ project license
├── makefile ........................................... make tasks manifest
├── readme.md .......................................... project documentation
├── netlify.toml ....................................... netlify config file
├── .travis.yml ........................................ travis ci config file
└── .editorconfig ...................................... common text editor configs
```

## Services

### demux

Demux is a backend infrastructure pattern for sourcing blockchain events to deterministically update queryable datastores and trigger side effects. 

Taking inspiration from the [Flux Architecture](https://facebook.github.io/flux/docs/in-depth-overview.html#content) pattern and [Redux](https://github.com/reduxjs/redux/), Demux was born out of the following qualifications:

1. A separation of concerns between how state exists on the blockchain and how it is queried by the client front-end
1. Client front-end not solely responsible for determining derived, reduced, and/or accumulated state
1. The ability for blockchain events to trigger new transactions, as well as other side effects outside of the blockchain
1. The blockchain as the single source of truth for all application state

<p align="center">
		<img src="docs/demux.png" width="600">
</p>

1. Client sends transaction to blockchain.
1. Action Watcher invokes Action Reader to check for new blocks.
1. Action Reader sees transaction in new block, parses actions.
1. Action Watcher sends actions to Action Handler.
1. Action Handler processes actions through Updaters and Effects.
1. Actions run their corresponding Updaters, updating the state of the Datastore.
1. Actions run their corresponding Effects, triggering external events.
1. Client queries API for updated data.

Learn more at https://github.com/EOSIO/demux-js.

We recomend using [EOS Local](https://github.com/eoscostarica/eos-local) to connect your demux to an EOSIO node running on your machine.

### graphql 

GraphQL is a query language for APIs and a runtime for fulfilling those queries with your existing data. GraphQL provides a complete and understandable description of the data in your API, gives clients the power to ask for exactly what they need and nothing more, makes it easier to evolve APIs over time, and enables powerful developer tools.

There are many reason for choosing GraphQL over other solutions, read [Top 5 Reasons to Use GraphQL](https://www.prisma.io/blog/top-5-reasons-to-use-graphql-b60cfa683511/).

__Move faster with powerful developer tools__

Know exactly what data you can request from your API without leaving your editor, highlight potential issues before sending a query, and take advantage of improved code intelligence. GraphQL makes it easy to build powerful tools like [GraphiQL](https://github.com/graphql/graphiql) by leveraging your API’s type system.

The GraphiQL instance on EOS Local is available at http://localhost:8088/console/api-explorer

<p align="center">
	<img src="docs/graphiql.png" width="600">
</p>

Learn more at https://graphql.org & https://www.howtographql.com

#### hasura 

Hasura GraphQL engine automatically generates your GraphQL schema and resolvers based on your tables/views in Postgres. You don’t need to write a GraphQL schema or resolvers.

The Hasura console gives you UI tools that speed up your data-modeling process, or working with your existing database. The console also automatically generates migrations or metadata files that you can edit directly and check into your version control.

Hasura GraphQL engine lets you do anything you would usually do with Postgres by giving you GraphQL over native Postgres constructs.

Learn more at https://hasura.io

### postgres

PostgreSQL is a powerful, open source object-relational database system with over 30 years of active development that has earned it a strong reputation for reliability, feature robustness, and performance.

- Postgres has a strongly typed schema that leaves very little room for errors. You first create the schema for a table and then add rows to the table. You can also define relationships between different tables with rules so that you can store related data across several tables and avoid data duplication.

- You can change tables in PostgreSQL without requiring to lock it for every operation. For example, you can add a column and set a default value quickly without locking the entire table. This ensures that every row in a table has the column and your codebase remains clean without needing to check if the column exists at every stage. It is also much quicker to update every row since Postgres doesn't need to retrieve each row, update, and put it back.

- Postgres also supports JSONB, which lets you create unstructured data, but with data constraint and validation functions to help ensure that JSON documents are more meaningful. The folks at Sisense have written a great blog with a detailed comparison of [Postgres vs MongoDB for JSON documents](https://www.sisense.com/blog/postgres-vs-mongodb-for-storing-json-data/).

- The newest round of performance comparisons of PostgreSQL and MongoDB produced a near repeat of the results from the first tests that proved PostgreSQL can outperform MongoDB.

<p align="center">
		<img src="docs/postgres.png" width="600">
</p>

Learn more at https://www.postgresql.org

### pgweb

Pgweb is a web-based database browser for PostgreSQL, written in Go and works on OSX, Linux and Windows machines. Main idea behind using Go for backend development is to utilize ability of the compiler to produce zero-dependency binaries for multiple platforms. Pgweb was created as an attempt to build very simple and portable application to work with local or remote PostgreSQL databases.

<p align="center">
	<img src="docs/pgweb.png" width="600">
</p>

Docker compose exposes a pgweb instance on http://localhost:8081 and also through http://pgweb.eoslocal.io with the nginx reverse-proxy.

### reactjs web client

In the services/frontend folder you will find a production ready frontend with Scatter and Lynx libraries ready for you to use. 

This frontend application uses Materail UI, this UI framework will allow you to build maintainable, scalable web and mobile interfaces.

<p align="center">
	<a href="https://www.youtube.com/watch?v=m1diVY4Uzjc">
		<img src="https://monosnap.com/image/KgKSu742SYRRC27HojRPvva5TlMu8j.png" width="600">
	</a>
</p>

### Material UI

<p align="center">
	<a href="https://www.youtube.com/watch?v=Akfp2X5QUXs">
		<img src="https://monosnap.com/image/NkiUDQ8CWcsthdPG7s7WTom3oneUMQ.png" width="600">
	</a>
</p>

#### components

- [react-app-rewired](https://github.com/timarney/react-app-rewired) for tweaking `create-react-app` configuration without ejecting.
- [reach-router](https://github.com/reach/router) for a more accessible router.
- state management with [rematch](https://github.com/rematch/rematch) to use `redux` best practices without all the boilerplate.
- [react-apollo](https://github.com/apollographql/react-apollo) react apollo client.
- [material-ui](https://github.com/mui-org/material-ui).
- [scatter-js](https://github.com/GetScatter/scatter-js).
- [eoslynx integration](https://developers.eoslynx.com/).

## Continuous Integration Process

- [TravisCI](https://travis-ci.org/) to run test and code style checks.
- [Netlify](https://netlify.com) for continuous delivery and creation of ephemeral test environments.

## EOS Documentation & Learning Resources

- https://developers.eos.io
- https://learn.eoscostarica.io

## Frequently Asked Questions

### Why Containers ?

The primary benefits of containers are efficiency and agility. Containers are orders of magnitude faster to provision, and much lighter-weight to build and define versus methods like omnibus software builds and full Virtual Machine images. Containers in a single OS are also more efficient at resource utilization than running a Hypervisor and guest OSs.

Efficiency and agility are good for everyone, but they become game-changers at scale. 

It also gives the ability to run distint versions of the different services like EOSIO on your laptop without conflicts.

Containers offer a logical packaging mechanism in which applications can be abstracted from the environment in which they actually run. This decoupling allows container-based applications to be deployed easily and consistently, regardless of whether the target environment is a private data center, the public cloud, or even a developer’s personal laptop. Containerization provides a clean separation of concerns, as developers focus on their application logic and dependencies, while IT operations teams can focus on deployment and management without bothering with application details such as specific software versions and configurations specific to the app.

For those coming from virtualized environments, containers are often compared with virtual machines (VMs). You might already be familiar with VMs: a guest operating system such as Linux or Windows runs on top of a host operating system with virtualized access to the underlying hardware. Like virtual machines, containers allow you to package your application together with libraries and other dependencies, providing isolated environments for running your software services. As you’ll see below however, the similarities end here as containers offer a far more lightweight unit for developers and IT Ops teams to work with, carrying a myriad of benefits.

<p align="center">
		<img src="docs/containers.png" width="600">
</p>

Learn more at https://cloud.google.com/containers/

### Why Database Migrations ?

- It enables a rock-solid deployment process because you are doing exactly the same when updating your local database, your development database, your QA database, your acceptance database and your production database. It’s always the same process and it can be automated.
- You can easily bring a (CI-)database to the point you want by loading a baseline backup and running all migration scripts until a certain point.
- If you do it right you have database versioning and change documentation included
- The approach encourages small changes at a time, leading to less risky deployments
- It enables and empowers continuous integration because you can easily transport your functional stat to different data sets (e.g. test data)
- You know exactly what’s happening. That’s in my opinion the greatest benefit of all, because it gives you confidence that what you’re delivering will work. It also gives you enormous flexibility and lets you solve any kind of challenge – even and especially ones which need specific business logic.

Learn more at https://dev.to/pesse/one-does-not-simply-update-a-database--migration-based-database-development-527d

### Who is using EOS Local ?

- [BeSpiral](http://bespiral.com).
- [EOS Rate](https://rate.eoscostarica.io).
- [DMeetup](https://github.com/eoscostarica/dmeetup).
- Add your project by creating a pull request.

## Contributing

We use a Kanban-style board. That's were we prioritize the work. [Go to Project Board](https://github.com/eoscostarica/eos-local/projects/3).

<p align="center">
	<img src="docs/project-board.png" width="600">
</p>

The main communication channels are [github issues](https://github.com/eoscostarica/eos-local/issues) and [EOS Costa Rica's Discord server](https://eoscostarica.io/discord). Feel to join and ask as many questions you may have.

Our weekly sync call is every Monday 1:00 AM UTC. [meet.eoscostarica.io](https:/meet.eoscostarica.io).

Contributing Guidelines https://developers.eoscostarica.io/docs/open-source-guidelines.

Please report bugs big and small by [opening an issue](https://github.com/eoscostarica/eos-local/issues)

## About EOS Costa Rica
<p align="center">
	<a href="https://eoscostarica.io">
		<img src="https://cdn.rawgit.com/eoscostarica/assets/574d20a6/logos/eoscolors-transparent.png" width="300">
	</a>
</p>

We challenge ourselves to provide the EOS platform with a strong geographical and political diversity by running the most robust EOS Block Producer possible from Costa Rica; We pledge to leverage our talent, experience, and sustainable internet resources to meet such an important challenge.

EOS Costa Rica supports the EOSIO community by maintaining and contributing to open source initiatives, meetups and workshops.

[eoscostarica.io](https://eoscostarica.io)

## License

MIT © [EOS Costa Rica](https://eoscostarica.io)  

## Contributors

<!-- ALL-CONTRIBUTORS-LIST:START - Do not remove or modify this section -->
<!-- prettier-ignore -->
| [<img src="https://avatars0.githubusercontent.com/u/391270?v=4" width="100px;"/><br /><sub><b>Gabo Esquivel</b></sub>](https://gaboesquivel.com)<br />[🤔](#ideas-gaboesquivel "Ideas, Planning, & Feedback") [📖](https://github.com/eoscostarica/eos-dapp-dev-env/commits?author=gaboesquivel "Documentation") [💻](https://github.com/eoscostarica/eos-dapp-dev-env/commits?author=gaboesquivel "Code") [👀](#review-gaboesquivel "Reviewed Pull Requests") | [<img src="https://avatars2.githubusercontent.com/u/349542?v=4" width="100px;"/><br /><sub><b>Daniel Prado</b></sub>](https://github.com/danazkari)<br />[💻](https://github.com/eoscostarica/eos-dapp-dev-env/commits?author=danazkari "Code") [📖](https://github.com/eoscostarica/eos-dapp-dev-env/commits?author=danazkari "Documentation") [🤔](#ideas-danazkari "Ideas, Planning, & Feedback") [👀](#review-danazkari "Reviewed Pull Requests") | [<img src="https://avatars1.githubusercontent.com/u/1179619?v=4" width="100px;"/><br /><sub><b>Jorge Murillo</b></sub>](https://github.com/murillojorge)<br />[🤔](#ideas-murillojorge "Ideas, Planning, & Feedback") [📖](https://github.com/eoscostarica/eos-dapp-dev-env/commits?author=murillojorge "Documentation") [🎨](#design-murillojorge "Design") [💻](https://github.com/eoscostarica/eos-dapp-dev-env/commits?author=murillojorge "Code") [👀](#review-murillojorge "Reviewed Pull Requests") | [<img src="https://avatars0.githubusercontent.com/u/5632966?v=4" width="100px;"/><br /><sub><b>Xavier Fernandez</b></sub>](https://github.com/xavier506)<br />[🤔](#ideas-xavier506 "Ideas, Planning, & Feedback") [📝](#blog-xavier506 "Blogposts") [📢](#talk-xavier506 "Talks") [🚇](#infra-xavier506 "Infrastructure (Hosting, Build-Tools, etc)") | [<img src="https://avatars2.githubusercontent.com/u/13205620?v=4" width="100px;"/><br /><sub><b>Rubén Abarca Navarro</b></sub>](https://github.com/rubenabix)<br />[🤔](#ideas-rubenabix "Ideas, Planning, & Feedback") [👀](#review-rubenabix "Reviewed Pull Requests") [💻](https://github.com/eoscostarica/eos-dapp-dev-env/commits?author=rubenabix "Code") | [<img src="https://avatars2.githubusercontent.com/u/15035769?v=4" width="100px;"/><br /><sub><b>jsegura17</b></sub>](https://github.com/jsegura17)<br />[💻](https://github.com/eoscostarica/eos-dapp-dev-env/commits?author=jsegura17 "Code") [👀](#review-jsegura17 "Reviewed Pull Requests") [🤔](#ideas-jsegura17 "Ideas, Planning, & Feedback") | [<img src="https://avatars1.githubusercontent.com/u/6147142?v=4" width="100px;"/><br /><sub><b>Leo Ribeiro</b></sub>](http://leordev.github.io)<br />[🤔](#ideas-leordev "Ideas, Planning, & Feedback") [👀](#review-leordev "Reviewed Pull Requests") |
| :---: | :---: | :---: | :---: | :---: | :---: | :---: |
| [<img src="https://avatars2.githubusercontent.com/u/16544451?v=4" width="100px;"/><br /><sub><b>Mariano Alvarez</b></sub>](https://github.com/mahcr)<br />[🤔](#ideas-mahcr "Ideas, Planning, & Feedback") [👀](#review-mahcr "Reviewed Pull Requests") | [<img src="https://avatars1.githubusercontent.com/u/1082127?v=4" width="100px;"/><br /><sub><b>Julien Lucca</b></sub>](http://lucca65.github.io)<br />[💻](https://github.com/eoscostarica/eos-dapp-dev-env/commits?author=lucca65 "Code") [👀](#review-lucca65 "Reviewed Pull Requests") [🤔](#ideas-lucca65 "Ideas, Planning, & Feedback") | [<img src="https://avatars2.githubusercontent.com/u/40245170?v=4" width="100px;"/><br /><sub><b>Edgar Fernandez</b></sub>](http://www.eoscostarica.io)<br />[🤔](#ideas-edgar-eoscostarica "Ideas, Planning, & Feedback") [📝](#blog-edgar-eoscostarica "Blogposts") [📢](#talk-edgar-eoscostarica "Talks") | [<img src="https://avatars3.githubusercontent.com/u/1288106?v=4" width="100px;"/><br /><sub><b>César Rodríguez</b></sub>](http://www.kesarito.com)<br />[🤔](#ideas-kesar "Ideas, Planning, & Feedback") | [<img src="https://avatars2.githubusercontent.com/u/1371207?v=4" width="100px;"/><br /><sub><b>Pacien Boisson</b></sub>](https://ngfar.io)<br />[🤔](#ideas-pakokrew "Ideas, Planning, & Feedback") | [<img src="https://avatars0.githubusercontent.com/u/17969875?v=4" width="100px;"/><br /><sub><b>Alex Gomory</b></sub>](https://github.com/GMory)<br />[💻](https://github.com/eoscostarica/eos-dapp-dev-env/commits?author=GMory "Code") [🤔](#ideas-GMory "Ideas, Planning, & Feedback") [📢](#talk-GMory "Talks") |
<!-- ALL-CONTRIBUTORS-LIST:END -->
Thanks goes to these wonderful people ([emoji key](https://github.com/kentcdodds/all-contributors#emoji-key)):

<!-- ALL-CONTRIBUTORS-LIST:START - Do not remove or modify this section -->
<!-- prettier-ignore -->
<!-- ALL-CONTRIBUTORS-LIST:END -->

This project follows the [all-contributors](https://github.com/kentcdodds/all-contributors) specification. Contributions of any kind welcome!
