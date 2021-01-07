from django.shortcuts import render, get_object_or_404, redirect
from django.http import HttpResponse
from django.contrib.auth import authenticate, login
from django.views.generic import View
from django.views.generic.edit import CreateView  
from django.views import generic
from .models import Algorithm
from .forms import UserForm, LoginUserForm, AddAlgForm


class DetailView(generic.DetailView):
    model = Algorithm
    template_name = 'algorithms/detail.html'

def index(request):
    all_algorithms = Algorithm.objects.all()
    context = {
        'all_algorithms' : all_algorithms,
    }
    return render(request, 'algorithms/index.html', context )
    # jesli chcemy cos ze słownika zawrzeć w template, to chodzi o klucz, czyli string w tym wypadku

def home(request):
   return render(request, 'algorithms/home.html' )

class UserFormView(View):
    form_class = UserForm
    template_name = 'algorithms/registration.html'

    # wyswietla formularz
    def get(self, request):
        form = self.form_class(None)
        return render(request, self.template_name, { 'form' : form })
    
    # przetwarzanie wpisanych danych
    def post(self, request):
        form = self.form_class(request.POST)

        if form.is_valid():
            user = form.save(commit=False)
            username = form.cleaned_data['username']
            password = form.cleaned_data['password']
            user.set_password(password)
            user.is_active=False
            user.save()

            if user is not None:
                return redirect('algorithms:wait')
            
        return render(request, self.template_name, { 'form' : form })


class LoginView(View):

    form_class = LoginUserForm
    template_name = 'algorithms/login.html'

    # wyswietla formularz
    def get(self, request):
        form = self.form_class(None)
        return render(request, self.template_name, { 'form' : form })
    
    # przetwarzanie wpisanych danych
    def post(self, request):
        form = self.form_class(request.POST)

        #if form.is_valid():
        username = request.POST['username']
        password = request.POST['password']

        user = authenticate(request, username = username, password = password)
        if user is not None:
            if user.is_active:
                login(request, user)
                return redirect('algorithms:home')
            
        return render(request, self.template_name, { 'form' : form })

def wait(request):
    return render(request, 'algorithms/wait-for-activate.html' )


#def handle_uploaded_file(f):  
#    with open('media/'+f.name, 'wb+') as destination:  
#        for chunk in f.chunks():  
#            destination.write(chunk)

#def addAlgView(request):  
#    if request.method == 'POST':
#        alg = AddAlgForm(request.POST, request.FILES)  
#        if alg.is_valid():
#            handle_uploaded_file(request.FILES['file'])  
#            return HttpResponse("File uploaded successfuly")  
#    else:  
#        alg = AddAlgForm()  
#        return render(request,"algorithms/add_alg.html",{'form' : alg}) 

class AddAlgorithm(CreateView):
    model = Algorithm
    fields = ['name', 'author', 'alg_file']
