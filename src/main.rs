use actix_files as fs;
use actix_web::{get, web, App, HttpResponse, HttpServer, Responder};
use std::{fs as std_fs, path::PathBuf};
use actix_files::NamedFile;
use actix_web::http::header;
use serde::Deserialize;

#[derive(Deserialize)]
struct PasswordForm {
    password: String,
}


#[get("/")]
async fn hello() -> impl Responder {
    HttpResponse::Ok().body("Hello world")
}

async fn home() -> impl Responder {
    let path = "thegrandline.html";
    let file = std_fs::read_to_string(path).expect("Cannot read HTML file");
    HttpResponse::Ok().content_type("text/html").body(file)
}

async fn johncpp() -> actix_web::Result<NamedFile>{
    let path: PathBuf = "private/john.cpp".into();
    Ok(NamedFile::open(path)?)
}

async fn inputjohn() -> actix_web::Result<NamedFile>{
    let path: PathBuf = "private/input.txt".into();
    Ok(NamedFile::open(path)?)
}

async fn check_password(form: web::Form<PasswordForm>) -> impl Responder {

    if form.password == "FORMAT" {
        HttpResponse::Found()
            .append_header((header::LOCATION, "/john"))
            .finish()
    } else {
        HttpResponse::NotFound().content_type("text/html").body("IT'S WRONG :(")
    }
}



#[actix_web::main]
async fn main() -> std::io::Result<()> {
    HttpServer::new(|| {
        App::new()
        .route("/thegrandline", web::get().to(home))
        .route("/pass", web::post().to(check_password))
        .route("/john", web::get().to(johncpp))
        .route("/inputforjohn", web::get().to(inputjohn))
        .service(fs::Files::new("/static", "./static")) // Serve static files
    })
    .bind(("127.0.0.1", 8080))?
    .run()
    .await
}
