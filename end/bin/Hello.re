open Lwt;
open Cohttp;
open Cohttp_lwt_unix;

// The project key of the project that contains the flag you
// created in the LaunchDarkly dashboard.
let project_key = "default";

// API for getting flags in a project.
// https://apidocs.launchdarkly.com/reference#list-feature-flags
let projects = "https://app.launchdarkly.com/api/v2/flags/" ++ project_key;

// The personal access token (PAT) you obtained from the LaunchDarkly dashboard.
// https://app.launchdarkly.com/settings/authorization
// TODO: Add your personal access token here.
let auth_token = "add_your_token_here";

// Set the Authorization header with the PAT.
let headers = Header.of_list([ ("Authorization", auth_token) ]);

// Make a call to the LaunchDarkly API to get the flags
// in our default project. We will print out the response code, response
// headers, and the raw response body JSON.
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
    }
  );

let () = {
  let body = Lwt_main.run(body);
  print_endline("Received body\n" ++ body);
};
