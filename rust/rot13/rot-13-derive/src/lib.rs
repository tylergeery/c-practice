extern crate proc_macro;
// extern crate quote;
extern crate syn;

use crate::proc_macro::TokenStream;
use quote::quote;

fn impl_rot13(ast: &syn::DeriveInput) -> TokenStream {
    let name = &ast.ident;
    let gen = quote! {
        impl Rot13 for #name {
            fn rot13(&self) {
                println!("rot13 called! called from {}", stringify!(#name));
            }
        }
    };

    gen.into()
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
