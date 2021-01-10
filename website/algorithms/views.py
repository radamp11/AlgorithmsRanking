from django.shortcuts import render, get_object_or_404, redirect
from django.http import HttpResponse
from django.contrib.auth import authenticate, login, logout
from django.views.generic import View, ListView, DetailView
from django.views.generic.edit import CreateView
from django.core.files import File
from .models import Algorithm, Outcome
from .forms import UserForm, LoginUserForm, CompareAlgorithms

import sys
sys.path.append('../FileManagement/')

from FileManagement import szkielet


class DetailView(DetailView):
    model = Algorithm
    template_name = 'algorithms/detail.html'

    def get_object(self):
        alg = super().get_object()

        if not alg.counted:
            dimensions = szkielet.generateResults("media/" + str(alg.alg_file))
            rownum = 1
            for dim in dimensions:
                with open('static/result_files/results' + str(dim) + '.txt', 'r') as myfile:
                    rownum = 1
                    for line in myfile:
                        out = Outcome()
                        out.algorithm = alg
                        out.dimension = dim
                        out.function = rownum
                        word_number = 1
                        for word in line.split():
                            if word_number == 1:
                                out.best = word
                            elif word_number == 2:
                                out.worst = word
                            elif word_number == 3:
                                out.median = word
                            elif word_number == 4:
                                out.mean_str = word
                                out.mean_float = float(word)
                            else:
                                out.std = word
                            
                            word_number = word_number + 1

                        rownum = rownum + 1
                        out.save()
                    
            alg.num_of_fun = rownum - 1
            alg.counted = True
            alg.save()
        return alg


class AlgorithmListView(ListView):
    model = Algorithm

    def get_queryset(self):
        return Algorithm.objects.all()


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

        if form.is_valid():
            username = request.POST['username']
            password = request.POST['password']

            user = authenticate(request, username = username, password = password)
            if user is not None:
                if user.is_active:
                    login(request, user)
                    return redirect('algorithms:home')
                else:       
                    return render(request, 'algorithms/wait-for-activate.html' )
            
        return render(request, self.template_name, { 'form' : form })

def wait(request):
    return render(request, 'algorithms/wait-for-activate.html' )


class AddAlgorithm(CreateView):
    model = Algorithm
    fields = ['name', 'author', 'alg_file']


def logoutView(request):
    logout(request)
    return render(request, 'algorithms/logout.html')


class CompAlgView(View):
    form_class = CompareAlgorithms
    template_name = 'algorithms/compare.html'

    # wyswietla formularz
    def get(self, request):
        form = self.form_class(None)
        return render(request, self.template_name, { 'form' : form })
    
    # przetwarzanie wpisanych danych
    def post(self, request):
        form = self.form_class(request.POST)
        if form.is_valid():
            alg1 = Algorithm.objects.get(pk = request.POST['algorithm1'])
            alg2 = Algorithm.objects.get(pk = request.POST['algorithm2'])
            print(alg1.alg_file)
            print(type(alg1.alg_file))
            print(type(alg2))
        return render(request, self.template_name, { 'form' : form })