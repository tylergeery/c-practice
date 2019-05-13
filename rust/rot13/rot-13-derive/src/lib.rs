extern crate proc_macro;

use crate::proc_macro::TokenStream;
use quote::quote;

fn do_rot_13() {

}

fn impl_rot13(ast: &syn::DeriveInput) -> TokenStream {
    let name = &ast.ident;
    match ast.data {
        syn::Data::Struct(ref data_struct) => {
            match data_struct.fields {
                // Structure with named fields (as opposed to tuple-like struct or unit struct)
                // E.g. struct Point { x: f64, y: f64 }
                syn::Fields::Named(ref fields_named) => {
                    // Iterate over the fields: `x`, `y`, ..
                    let gen = quote! {
                        impl Rot13 for #name {
                            fn rot13(&self) {
                                println!("rot13 called! called from {}", stringify!(#name));
                            }
                        }
                    };

                    for f in &fields_named.named {
                        // let tokens = TokenStream::new();
                        // f.ty.to_tokens(tokens);
                        println!("found f.ident: {:?} with type", &f.ident);
                    }

                    return gen.into();
                    // for field in fields_named.named.iter() {
                    //     let field_name = &field.ident;
                    //
                    //
                    // }
                },
                _ => (),
            }
        },
        _ => panic!("Must be a struct"),
    }

    TokenStream::new()
}

#[proc_macro_derive(Rot13)]
pub fn rot13_derive(input: TokenStream) -> TokenStream {
    // Parse the inut
    let ast = syn::parse(input).unwrap();

    impl_rot13(&ast)
}

#[cfg(test)]
mod tests {
    #[test]
    fn it_works() {
    }
}
