# Getting started with the LaunchDarkly REST API and ReasonML

<!-- Metadata
This code was tested on a Mac. It has not been tested on Linux or Windows.

Opportunities:
- Add a sister document demonstrating an integration with ReScript and LD JavaScript library.
- Add an action based on the results of the first call.
- Provide scenarios descriptions around when someone may want to use this content.
 -->

## Overview

The goal of this tutorial to get you started using the LaunchDarkly REST APIs from a native ReasonML project.

The [Launch Darkly REST API](https://apidocs.launchdarkly.com/reference) is used for developing custom integrations with LaunchDarkly and for data export. [ReasonML](https://reasonml.github.io/docs/en/what-and-why) is a new syntax for the [OCaml](https://ocaml.org/) programming language.  Reason is a functional language that uses the strong OCaml type system to provide maintainable and performant code.

Let’s get into your first LaunchDarkly integration with Reason. Before we get started with Reason, we’ll need to onboard to LaunchDarkly.

## Get setup with the LaunchDarkly dashboard

The following steps assume that you haven’t onboarded to LaunchDarkly. If you already have a default project setup with a feature flag, you can skip to the part where you get an access token.

We’ll first need to sign up for LaunchDarkly and create your first feature flag in the dashboard. Our goal will be to get information about this feature flag you create via the LaunchDarkly REST API.

1. Go to the [LaunchDarkly sign up](https://launchdarkly.com/pricing/) page and select the **Start Trial** option. Don’t worry, they don’t ask for your credit card. This will drop you into the LaunchDarkly quick start experience.

2. Create a feature flag named **myflag**.

Next, we’ll need to get an access token. You can create an access token in the **Account setting** > [Authorization](https://app.launchdarkly.com/settings/authorization) tab in the dashboard. Create an access token and copy it from the dashboard before you navigate away from the page as you can only copy the access token once. You will need this access token to authenticate your requests to the REST API.

## Setup Reason Hello World

We’ll use the [hello-reason](https://github.com/esy-ocaml/hello-reason) repository as a starting point for this exercise. This is the same repository used in the [Reason installation](https://reasonml.github.io/docs/en/installation) topic. I’ve copied the hello-reason repository so that you can see a before and after state of the repository in case you have any issues.

First, let’s install **esy**, the Reason package manager.

`npm install -g esy`

Next let’s get a copy of the ld-reason-get-started repo which contains copies of hello-reason repo. The `/begin` directory will be our working directory.

`git clone https://github.com/MIchaelMainer/ld-reason-get-started.git`

`cd ld-reason-get-started/begin`

We’ll now need to install the dependencies in the project.

`esy`

Next we will compile and run Hello.exe.

`esy x Hello`

At this point, we can successfully run the Reason Hello World sample. Now let’s make the LaunchDarkly integration.

## Integrate with LaunchDarkly

`esy` doesn’t have a native HTTP client package for Reason so we will need to use one of the OCaml HTTP clients. For our purposes, we’ll use the [ocaml-cohttp](https://github.com/mirage/ocaml-cohttp) client. Let’s install the cohttp client into our project as well as the SSL library.

`esy add @opam/cohttp-lwt-unix`

`esy add @opam/lwt_ssl`

Next, we need to add the libraries to the Hello dune file located at `./bin/dune`. Make the following change to the libraries stanza:

```
(libraries console.lib lib cohttp-lwt-unix lwt)
```

Now we are ready to integrate with LaunchDarkly. Let’s remove the content in the `./bin/Hello.re` file. All of the following code will be added to Hello.re. Next, let’s add the referenced HTTP client packages to Hello.re:

```
open Lwt;
open Cohttp;
open Cohttp_lwt_unix;
```

Add the following lines to specify the API to get LaucnhDarkly flags from a project. In this case, we are using the default project created when using the LaunchDarkly quickstart guide.

```
let project_key = "default";
let projects = "https://app.launchdarkly.com/api/v2/flags/" ++ project_key;
```

Now we need to setup the Authorization header so our requests can be authenticated with the LaunchDarkly API. Add your access token to `auth_token`.

```
let auth_token = "add_your_token_here";
let headers = Header.of_list([ ("Authorization", auth_token) ]);
```

Next, we will add the client sample from the ocaml-cohttp readme. The only difference from the source sample is that we are using Reason instead of OCaml, the LaunchDarkly project flags endpoint, and the adding the headers collection with our Authorization header.

```
let body =
    Client.get(~headers, Uri.of_string(projects))
    >>= (
    ((resp, body)) => {
        let code = resp |> Response.status |> Code.code_of_status;
        Printf.printf("Response code: %d\n", code);
        Printf.printf(
            "Headers: %s\n",
            resp |> Response.headers |> Header.to_string,
        );
        body
        |> Cohttp_lwt.Body.to_string
        >|= (
            body => {
                Printf.printf("Body of length: %d\n", String.length(body));
                body;
            }
        );
    });

let () = {
    let body = Lwt_main.run(body);
    print_endline("Received body\n" ++ body);
};
```

Now we are ready to run the program.

`esy x Hello`

You should see JSON data with your default project flag information printed to the console. Congratulations, you’ve completed your first native Reason integration with the LaunchDarkly REST API! Compare the contents of the `/begin` and `/end` directories if you encountered any issues. The `/end` directory contains all of the code changes in case you needed it.